#include "BufferedSoftSerial.h"
#include "../../Module/ModuleWrapper.h"
#include "XString.h"

class IM920SoftSerial : public ModuleWrapper
{
    //received format
    //xx,yyyy,zz: message...
    //xx: Node number
    //yyyy: ID number
    //zz: RSSI value
    enum class MessageType
    {
        Node,
        ID,
        RSSI,
        Message
    };

    BufferedSoftSerial ss_;

    MessageType type_ = MessageType::Node;

    char c = '\0';
    xString received_ = "";

    bool reading_ = false;
    bool readingMessage_ = false;
    bool sendToSerial_ = false;
    bool completeReceive_ = false;

    Callback<void()> callback_;

public:
    IM920SoftSerial(const xString &name, PinName tx, PinName rx)
        : ss_(tx, rx),
          ModuleWrapper(name)
    {
        ss_.baud(19200);
    }

    void update()
    {
        while (ss_.readable())
        {
            c = static_cast<char>(ss_.getc());

            switch (type_)
            {
            case MessageType::Node:
                receiveNode();
                break;

            case MessageType::ID:
                receiveID();
                break;

            case MessageType::RSSI:
                receiveRSSI();
                break;

            case MessageType::Message:
                receiveMessage();
                break;
            }
        }

        if (completeReceive_)
        {
            callback_.call();
            completeReceive_ = false;
        }
    }

    void attach(void (*fptr)() = NULL)
    {
        callback_ = callback(fptr);
    }
    
    template <typename T>
    void attach(T *tptr, void (T::*mptr)())
    {
        if ((mptr != NULL) && (tptr != NULL))
        {
            callback_ = callback(tptr, mptr);
        }
    }

    bool hasValue()
    {
        return !received_.empty();
    }

    xString received() const
    {
        if (sendToSerial_)
        {
            printf("[Debug Received]");
            printf("%s", received_.c_str());
            printf("\r\n");
        }
        return received_;
    }

    void clear()
    {
        received_.clear();
    }

    void setSendToSerial(bool send)
    {
        sendToSerial_ = send;
    }

    virtual void initialize() override
    {
        available = true;
    }

    virtual bool isAvailable() override
    {
        return available;
    }

private:
    void receiveNode()
    {
        if (c == ',')
        {
            type_ = MessageType::ID;
        }
    }

    void receiveID()
    {
        if (c == ',')
        {
            type_ = MessageType::RSSI;
        }
    }

    void receiveRSSI()
    {
        if (c == ' ')
        {
            received_.clear();
            type_ = MessageType::Message;
        }
    }

    void receiveMessage()
    {
        if (c == '\r' || c == '\n')
        {
            type_ = MessageType::Node;
            completeReceive_ = true;
        }
        else
        {
            received_ += c;
        }
    }
};