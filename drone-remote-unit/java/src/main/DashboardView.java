package main;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;

public class DashboardView extends JFrame implements ActionListener {

    private DashboardController controller; 

    public static final int HANGAR_NORMAL    = 0;
    public static final int HANGAR_PREALARM  = 1;
    public static final int HANGAR_ALARM     = 2;

    private JButton btnTakeOff;
    private JButton btnLand;
    
    private JTextField txtDistance;
    private JTextField txtTemperature;
    private JTextField txtStateLabel;
    
    private JPanel mainPanel;
    private JLabel lblAlarmStatus;

    private static final int WIDTH = 600;
    private static final int HEIGHT = 300;

    public DashboardView() {
        super(" Drone Hangar Remote Unit ");
        setSize(WIDTH, HEIGHT);
        this.setResizable(false);

        mainPanel = new JPanel();
        mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
        mainPanel.add(Box.createRigidArea(new Dimension(0, 20))); 

        JPanel infoPanel = new JPanel();
        infoPanel.setLayout(new BoxLayout(infoPanel, BoxLayout.LINE_AXIS));
        
        txtStateLabel = createOutputField("Drone State:", infoPanel, 150);
        txtDistance = createOutputField("Distance (cm):", infoPanel, 80);
        txtTemperature = createOutputField("Temp (°C):", infoPanel, 80);

        mainPanel.add(infoPanel);
        mainPanel.add(Box.createRigidArea(new Dimension(0, 15)));

        lblAlarmStatus = new JLabel(" SYSTEM NORMAL ");
        lblAlarmStatus.setOpaque(true);
        lblAlarmStatus.setBackground(Color.GREEN);
        lblAlarmStatus.setForeground(Color.BLACK);
        lblAlarmStatus.setFont(new Font("Arial", Font.BOLD, 18));
        lblAlarmStatus.setAlignmentX(CENTER_ALIGNMENT);
        lblAlarmStatus.setBorder(BorderFactory.createLineBorder(Color.BLACK, 1));

        lblAlarmStatus.setPreferredSize(new Dimension(500, 40));
        lblAlarmStatus.setMaximumSize(new Dimension(500, 40));
        
        mainPanel.add(lblAlarmStatus);
        mainPanel.add(Box.createRigidArea(new Dimension(0, 20)));

        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.X_AXIS));
        
        btnTakeOff = new JButton("Request TAKE-OFF");
        btnTakeOff.setEnabled(false);
        btnTakeOff.addActionListener(this);
        
        btnLand = new JButton("Request LANDING");
        btnLand.setEnabled(false);
        btnLand.addActionListener(this);

        buttonPanel.add(btnTakeOff);
        buttonPanel.add(Box.createRigidArea(new Dimension(20, 0))); 
        buttonPanel.add(btnLand);
        
        mainPanel.add(buttonPanel);
        mainPanel.add(Box.createRigidArea(new Dimension(0, 20))); 
        
        setContentPane(mainPanel);

    }

    private JTextField createOutputField(String labelText, JPanel parent, int width) {
        parent.add(new JLabel(labelText + " "));
        JTextField field = new JTextField("--");
        field.setEditable(false);
        field.setPreferredSize(new Dimension(width, 25));
        field.setMaximumSize(new Dimension(width, 25));
        field.setHorizontalAlignment(JTextField.CENTER);
        parent.add(field);
        parent.add(Box.createRigidArea(new Dimension(10, 0)));
        return field;
    }

    public void display() {
        SwingUtilities.invokeLater(() -> {
            this.setVisible(true);
        });
    }

    public void registerController(DashboardController contr) {
        this.controller = contr;
    }

    public void updateSensors(double distance, double temp) {
        SwingUtilities.invokeLater(() -> {
            txtDistance.setText(String.format("%.1f", distance));
            txtTemperature.setText(String.format("%.1f", temp));
        });
    }

    public void updateDroneStatus(String label) {
        SwingUtilities.invokeLater(() -> {
            txtStateLabel.setText(label);
        });
    }

    public void updateHangarCondition(int hangarStatus) {
        SwingUtilities.invokeLater(() -> {
            switch (hangarStatus) {
                case HANGAR_NORMAL:
                    lblAlarmStatus.setText(" SYSTEM NORMAL ");
                    lblAlarmStatus.setBackground(Color.GREEN);
                    lblAlarmStatus.setForeground(Color.BLACK);
                    break;

                case HANGAR_PREALARM:
                    lblAlarmStatus.setText(" PRE-ALARM: TEMP HIGH ");
                    lblAlarmStatus.setBackground(Color.ORANGE);
                    lblAlarmStatus.setForeground(Color.BLACK);
                    break;

                case HANGAR_ALARM:
                    lblAlarmStatus.setText(" ALARM ACTIVE - LOCKED ");
                    lblAlarmStatus.setBackground(Color.RED);
                    lblAlarmStatus.setForeground(Color.WHITE);
                    break;
                
                default:
                    lblAlarmStatus.setText(" UNKNOWN STATE ");
                    lblAlarmStatus.setBackground(Color.GRAY);
            }
        });
    }

    public void updateControlsState(int droneState, int hangarStatus) {
        SwingUtilities.invokeLater(() -> {

            boolean emergencyActive = (hangarStatus == HANGAR_PREALARM || hangarStatus == HANGAR_ALARM);

            if (emergencyActive) {
                btnTakeOff.setEnabled(false);
                btnLand.setEnabled(false);
            } else {
                switch (droneState) {
                    case MonitoringAgent.STATE_REST: 
                        btnTakeOff.setEnabled(true);
                        btnLand.setEnabled(false);
                        break;
                        
                    case MonitoringAgent.STATE_OUT:
                        btnTakeOff.setEnabled(false);
                        btnLand.setEnabled(true);
                        break;
                        
                    default:
                        btnTakeOff.setEnabled(false);
                        btnLand.setEnabled(false);
                        break;
                }
            }
        });
    }

    @Override
    public void actionPerformed(ActionEvent ev) {
        try {
            if (ev.getSource() == btnTakeOff) {
                controller.requestTakeOff();
                btnTakeOff.setEnabled(false); 
            } else if (ev.getSource() == btnLand) {
                controller.requestLanding();
                btnLand.setEnabled(false);
            }
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}