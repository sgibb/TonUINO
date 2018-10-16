# TonUINO

Die DIY Musikbox (nicht nur) für Kinder

Idee und Schaltplan von Thorsten Voss https://github.com/xfjx/TonUINO

# Genutzte Bibliotheken:
- https://github.com/bxparks/AceButton
- https://github.com/miguelbalboa/rfid
- https://github.com/Makuna/DFMiniMp3

# Veränderungen
- Wechsel von JC_Button auf AceButton für das Ansprechen der Buttons.
- Funktionsreduktion:
    - keine Adminfunktionen für die Karten
    - keine Speicherung der Titelnummer auf den Karten
    - keine Unterstützung von Abspiel-/Hörbuchmodus etc. (Hörbuchmodus ist der
        Standard)

# Bedienung
- Lauter/Leiser: langer Druck auf die Buttons A1/A2
- Nächster/vorheriger Titel: kurzer Druck auf die Buttons A1/A2
- Start/Pause: kurzer Druck auf Button A0
- Karte programmieren: 5x kurzer Druck auf A0, dann A1/A2 um Ordner zu wechseln,
    einmaliger Druck A0 um in den Abspielmodus zurückzukehren
