#include "SDCard/SDCard.h"
#include "ModuleWrapper.h"
#include "XString.h"

class SDCardWrapper : public ModuleWrapper
{

    SDCard sd_;
    FILE *fp_;

    int mounted_ = 0;
    int opened_ = 0;

public:
    SDCardWrapper(const xString &name, PinName miso, PinName mosi, PinName sck, PinName cs)
        : sd_(miso, mosi, sck, cs),
          ModuleWrapper(name)
    {
    }

    virtual void initialize() override
    {
        mounted_ = sd_.mounting();
    }

    bool isInitialized()const{
        return mounted_;
    }

    virtual bool isAvailable() override
    {
        return (mounted_ == 1 && opened_ == 1 && fp_ != nullptr);
    }

    void open(const xString &filepath)
    {
        const auto path = "/fs/" + filepath;
        opened_ = sd_.opening_file("/", path.c_str(), "a+");

        fp_ = sd_.getFile();
    }

    void close()
    {
        sd_.closing_file();
        sd_.unmounting();
    }

    void write(const xString& str){
        if(fp_!=nullptr){
            fprintf(fp_, str.c_str());
        }
    }
};