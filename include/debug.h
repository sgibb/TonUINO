#ifdef DEBUG
#define SERIALBEGIN(...)    Serial.begin(__VA_ARGS__);
#define DPRINT(...)         Serial.print(__VA_ARGS__); Serial.flush();
#define DPRINTLN(...)       Serial.println(__VA_ARGS__); Serial.flush();
#define DPRINTF(...)        Serial.print(F(__VA_ARGS__)); Serial.flush();
#define DPRINTLNF(...)      Serial.println(F(__VA_ARGS__)); Serial.flush();
#define DELAY(...)          delay(__VA_ARGS__);
#define DVPRINTLNF(...)     Serial.print(F(#__VA_ARGS__" = ")); Serial.println(__VA_ARGS__); Serial.flush();
#else
#define SERIALBEGIN(...)
#define DPRINT(...)
#define DPRINTLN(...)
#define DPRINTF(...)
#define DPRINTLNF(...)
#define DELAY(...)
#define DVPRINTLNF(...)
#endif
