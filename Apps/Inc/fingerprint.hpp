#ifndef FINGERPRINT_H
#define FINGERPRINT_H

#include "gui.hpp"

enum FingerprintResult
{
    FG_PASS = 0,
    FG_FAIL,
    FG_BadImage,
    FG_ERROR,
    FG_TIMEOUT,
    FG_WAIT
};

struct FingerprintRequest
{
    gui::Display* dis;
    gui::Window* wn_src;
    void (*result_callback)(FingerprintResult, FingerprintRequest*);
};

class Fingerprint
{
public:
    static void Authen(FingerprintRequest* req);
    static void Enroll(FingerprintRequest* req);
    static void cancelOperation();

    static void clearAllFinger(FingerprintRequest* req);
};


#endif //FINGERPRINT_H
