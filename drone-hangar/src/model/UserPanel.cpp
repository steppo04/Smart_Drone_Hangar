#include "UserPanel.h"
#include <Arduino.h>
#include "config.h"
#include "HWPlatform.h"

 UserPanel::UserPanel(HWPlatform* pHW) {
    pLcd = pHW->getUserPanelLcd();
    pButtonReset = pHW->getResetButton();
}
void UserPanel::init() {
    pLcd->init();
    pLcd->backlight();
    pLcd->clear();

    displayDroneInside();
}

void UserPanel::displayDroneInside() {
    pLcd->setCursor(0, 0); 
    pLcd->print("DRONE INSIDE");
}

void UserPanel::displayTakeOff() {
    pLcd->clear();
    pLcd->setCursor(0, 0); 
    pLcd->print("TAKE OFF");
}

void UserPanel::displayLanding() {
    pLcd->clear();
    pLcd->setCursor(0, 0); 
    pLcd->print("LANDING");
}

void UserPanel::displayDroneOut() {
    pLcd->clear();
    pLcd->setCursor(0, 0); 
    pLcd->print("DRONE OUT");
}

void UserPanel::displayAlarm() {
    pLcd->clear();
    pLcd->setCursor(0, 0); 
    pLcd->print("!!!!! ALARM !!!!!");
}

void UserPanel::sync() {
    // Aggiorna lo stato interno del componente Button (debouncing, ecc.)
    pButtonReset->sync(); 
    // Legge il nuovo stato del pulsante
    resetPressed = pButtonReset->isPressed(); 
}

bool UserPanel::pressedReset() {
    // Restituisce l'ultimo stato letto
    return resetPressed; 
}