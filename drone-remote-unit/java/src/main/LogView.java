package main;


import java.util.Date;

import javax.swing.*;

class LogView extends JFrame  {

	private JTextArea log;

	public LogView(){
		super("Log ");
		setSize(800,800);
		this.setResizable(false);
		JPanel mainPanel = new JPanel();
		log = new JTextArea(50,60);
		log.setEditable(false);
		JScrollPane scrol = new JScrollPane(log);
		mainPanel.add(scrol);
		this.getContentPane().add(mainPanel);
	}
	
	public void log(String msg){
		SwingUtilities.invokeLater(() -> {
			String date = new Date().toString();
			log.append("["+date+"] "+ msg +"\n");
		});
	}
	
	public void display() {
		SwingUtilities.invokeLater(() -> {
			this.setVisible(true);
		});
	}
}
