package main;

public class MonitoringAgent extends Thread {

    private SerialCommChannel channel;
    private DashboardView view;
    private LogView logger;
    
    static final String MSG_PREFIX  = "dh:"; 
    static final String MSG_STATUS  = "st:";

    public static final int STATE_REST        = 0;
    public static final int STATE_TAKING_OFF  = 1;
    public static final int STATE_OUT         = 2;
    public static final int STATE_LANDING     = 3;

    public static final int HANGAR_NORMAL     = 0;
    public static final int HANGAR_PREALARM   = 1;
    public static final int HANGAR_ALARM      = 2;

    private static final String[] DRONE_STATE_LABELS = {
        "INSIDE (Rest)", 
        "TAKING OFF", 
        "OUTSIDE (Operating)", 
        "LANDING"
    };

    public MonitoringAgent(SerialCommChannel channel, DashboardView view, LogView log) throws Exception {
        this.view = view;
        this.logger = log;
        this.channel = channel;
    }
    
    @Override
    public void run(){
        System.out.println("[Agent] Monitoring started...");
        
        while (true){
            try {
                String msg = channel.receiveMsg();

                if (msg.startsWith(MSG_PREFIX)){
                    String payload = msg.substring(MSG_PREFIX.length()); 

                    if (payload.startsWith(MSG_STATUS)){
                        handleStatusMessage(payload.substring(MSG_STATUS.length()));
                    }
                } else {
                    this.logger.log("ARDUINO_DBG: " + msg);
                }
            } catch (Exception ex){
                ex.printStackTrace();
                System.err.println("[Agent] Error processing message.");
            }
        }
    }

    private void handleStatusMessage(String args) {
        try {
            String[] elems = args.split(":");

            if (elems.length >= 4) {
                int droneStateCode = Integer.parseInt(elems[0]);
                double distance    = Double.parseDouble(elems[1]);
                double temperature = Double.parseDouble(elems[2]);
                int hangarStatusCode = Integer.parseInt(elems[3]);

                String statusLabel = (droneStateCode >= 0 && droneStateCode < DRONE_STATE_LABELS.length) 
                                     ? DRONE_STATE_LABELS[droneStateCode] 
                                     : "UNKNOWN";

                view.updateDroneStatus(statusLabel); 

                view.updateSensors(distance, temperature);

                view.updateHangarCondition(hangarStatusCode);

                view.updateControlsState(droneStateCode, hangarStatusCode);
            }
        } catch (NumberFormatException e) {
            System.err.println("[Agent] Error parsing numbers: " + args);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}