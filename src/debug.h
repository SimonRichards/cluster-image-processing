
#ifdef DEBUG
inline void info(string msg) {
    cout << msg << endl;
}
#else
#define info(x)
#endif
