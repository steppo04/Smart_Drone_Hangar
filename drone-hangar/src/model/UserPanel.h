#ifndef USERPANEL_H
#define USERPANEL_H

#include "config.h"
#include "HWPlatform.h"

// Dichiarazioni forward dei tipi hardware usati
class LiquidCrystal_I2C; // LCD I2C
class Button;           // Pulsante RESET
class HWPlatform;       // Piattaforma Hardware

class UserPanel {
private:
    // Componenti di Output (LCD)
    LiquidCrystal_I2C* pLcd; 
    
    // Componenti di Input (Pulsante)
    Button* pButtonReset;
    bool resetPressed; // Stato memorizzato del pulsante RESET

public:
    /**
     * @brief Costruttore: Ottiene i puntatori ai componenti dall'HWPlatform.
     */
    UserPanel(HWPlatform* pHW);
    
    /**
     * @brief Inizializza l'LCD e imposta lo stato iniziale dei LED.
     */
    void init();

    // --- Metodi di Output LCD ---
    
    void displayDroneInside();
    void displayTakeOff();
    void displayLanding();
    void displayDroneOut();
    void displayAlarm();
    // --- Metodi di Input Pulsante ---
    
    /**
     * @brief Sincronizza lo stato del pulsante RESET con l'hardware.
     * DEVE essere chiamato regolarmente (es. nel loop).
     */
    void sync(); 
    
    /**
     * @brief Restituisce true se il pulsante RESET è stato premuto.
     */
    bool pressedReset();
};

#endif // USERPANEL_H