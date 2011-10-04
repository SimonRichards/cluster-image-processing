bool received = false, processed = false;

int imageQueueLength = 0;
bool noImage = false;

inline receive() {
    imageQueueLength++
}

inline send() {
   imageQueueLength-- 
}

inline deconvolve() {
    if
        :: imageQueueLength != 1 -> noImage = true
        :: skip
    fi
}

active proctype connection() {
    start:
            receive();
            received = true;
            processed = false;
            processed;
            send();
            goto start
}

active proctype filter() {
    start:
    progress:
            received;
            received = false;
            deconvolve();
            processed = true;
            goto start
}


never {
    do
        :: imageQueueLength > 1 -> break
        :: true -> skip
    od
}
