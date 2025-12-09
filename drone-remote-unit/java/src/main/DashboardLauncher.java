package main;

class DashboardLauncher   {

	static DashboardView view = null;
	static LogView log = null;

	public static void main(String[] args) throws Exception {
		view = new DashboardView();
		log = new LogView();
		
		String portName = "COM9";
		DashboardController contr = new DashboardController(portName,view,log);
		view.registerController(contr);

		
		view.display();
		log.display();
	}
}