package main;

public class DashboardController {

    
    private static final String CMD_TAKEOFF = "cmd:takeoff";
    private static final String CMD_LAND    = "cmd:land";
    
    private SerialCommChannel channel;
    private DashboardView view;
    private LogView logger;
    
    public DashboardController(String port, DashboardView view, LogView logger) throws Exception {
        this.view = view;
        this.logger = logger;
        
        this.channel = new SerialCommChannel(port, 115200);       
        
        new MonitoringAgent(channel, view, logger).start();

        System.out.println("[Controller] Waiting for Arduino reboot...");     
        try {
            Thread.sleep(3000);
        } catch (InterruptedException ex) {
            ex.printStackTrace();
        }
        System.out.println("[Controller] Ready.");       
    }

    public void requestTakeOff() {
        System.out.println("[Controller] Sending command: TAKEOFF");
        channel.sendMsg(CMD_TAKEOFF);
    }

    public void requestLanding() {
        System.out.println("[Controller] Sending command: LAND");
        channel.sendMsg(CMD_LAND);
    }
}

