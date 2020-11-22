#include "SDCard.h"
 
SDCard::SDCard(PinName miso, PinName mosi, PinName sck, PinName cs) : __blockDevice(miso, mosi, sck, cs), __fileSystem("fs")
{
    __cs = cs;
}
 
int SDCard::mounting(){
    // Try to mount the filesystem
    fflush(stdout);
 
    __err = __fileSystem.mount(&__blockDevice);
    if (__err) {
        // Reformat if we can't mount the filesystem
        // this should only happen on the first boot
        fflush(stdout);
        
        __err = __fileSystem.reformat(&__blockDevice);
        
        return -1;
    }
    return 1;
} 
 
 
int SDCard::opening_file(char *path, const char* filename, const char* mode){
    char filetoopen[100];
    // Open the numbers file
    strcpy(filetoopen, path); 
    strcat(filetoopen, filename);
    fflush(stdout);
    __f = fopen(filetoopen, mode);
    if (!__f) {
        // Create the numbers file if it doesn't exist
        fflush(stdout);
        return -1;
    }
    return 1;
}
 
int SDCard::closing_file(void){
    // Close the file which also flushes any cached writes
    fflush(stdout);
    __err = fclose(__f);
    
    if (__err < 0) {
        return -1;
    }
    return 1;
}
 
int SDCard::unmounting(){
    fflush(stdout);
    __err = __fileSystem.unmount();
    
    if (__err < 0) {
        return -1;
    }
    
    fflush(stdout);
    __err = __blockDevice.deinit();
    
    if (__err) {
        return -1;
    }
    return 1;
}
 
FILE * SDCard::getFile(void){
    return __f;
}
            