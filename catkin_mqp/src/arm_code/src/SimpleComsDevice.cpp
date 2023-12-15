#include <vector>
#include <unordered_map>
//#include "PacketType.h"
#include "FloatPacketType.h"
//#include "Runnable.h"

#include <iostream>
#include <memory>
#include <cassert>
//#include <windows.h>
//#include <process.h>
//#include "hidapi.h"
#include <hidapi/hidapi.h>
//#include <libusb.h>
//#include <hidapi_libusb.h>

#include <memory>
#include <thread>
#include <complex>
#include <valarray>

#define _USE_MATH_DEFINES
# define M_PI           3.14159265358979323846  /* pi */

//also in hid.c
struct hid_device_;
typedef struct hid_device_ hid_device; /**< opaque hidapi structure */

typedef std::complex<float> Complex;
typedef std::valarray<Complex> CArray;


//definitions
static bool connected = false;
static bool virtualv = false;
static int readTimeout = 100;
static bool isTimedOut = false;
    

//going to open it in here instead of Robot
//device:
//from test.c
// Open the device using the VID, PID,
// and optionally the Serial number.
////handle = hid_open(0x4d8, 0x3f, L"12345");
unsigned short vid = 0x16c0;
unsigned short pid = 0x0486;
static hid_device * handle = hid_open(vid, pid, NULL); //from hid.c

//HIDPacketComs extends this, which calls read + write, which calls hidDevice.read+write, 
//which calls HidApi.read + write
//which calls hid_api.read + write
class SimpleComsDevice {
    public:
    /**
     * Constructor
     * technically need because of how it's used in Robot.cpp
     * also not so abstract anymore, TODO rename
    */
    /*SimpleComsDevice(){
        //dont feel a need to define any atrributes from Robot
    }*/

    

    // A Functor
    struct Runnable
    {
        Runnable() {  }
    
        // This operator overloading enables calling
        // operator function () on objects of increment
        int operator () () const {
            while (SimpleComsDevice::getConnected()) {
                try {
                    std::vector<FloatPacketType> pollingQueue = SimpleComsDevice::getPollingQueue();
                    for (int i = 0; i < pollingQueue.size(); i++) {
                        FloatPacketType pollingPacket = pollingQueue[i];
                        if (pollingPacket.sendOk()){
                            SimpleComsDevice::process(pollingPacket); //whys this the part I"m struggling with
                        }
                    } 
                } catch (const std::exception& e) {
                    printf("connect thread exception: ");
                    printf(e.what());
                } 
                try {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                } catch (const std::exception& e1) {
                    printf("connect thread sleep exception: ");
                    printf(e1.what());
                    SimpleComsDevice::setConnected(false);
                } 
            } 
            
            SimpleComsDevice::disconnectDeviceImp();
            printf("SimplePacketComs disconnect");
            return 0;
        };

    };
    
   

    static bool validHandle(hid_device * handle){
        if (!handle) {
            printf("unable to open device\n");
            hid_exit();
            return 1;
        }
    }

    static std::unordered_map<int, std::vector<Runnable>> events;
    
    
    
    static std::vector<FloatPacketType> pollingQueue;
    
    
    
    //String name = "SimpleComsDevice";
    
    
    static void addPollingPacket(FloatPacketType packet) {
        if (!(getPacket((int)packet.idOfCommand) == nullptr)){
            throw("Only one packet of a given ID is allowed to poll. Add an event to recive data"); 
        }
        pollingQueue.push_back(packet);
    }
    
    /**
     * given id
     * return FloatPacketType pointer corresponding to it
    */
    static FloatPacketType* getPacket(int ID) {
        for (FloatPacketType q : pollingQueue) {
            if (q.idOfCommand == ID){
                return &q; 
            }
        } 
        return nullptr;
    }
    
    /*
    public void removeEvent(int id, Runnable event) {
        if (this.events.get(id) == null)
        this.events.put(id, new ArrayList<>()); 
        ((ArrayList)this.events.get(id)).remove(event);
    }
    
    public void addEvent(int id, Runnable event) {
        if (this.events.get(id) == null)
        this.events.put(id, new ArrayList<>()); 
        ((ArrayList<Runnable>)this.events.get(id)).add(event);
    }
    
    public ArrayList<int> getIDs() {
        ArrayList<int> ids = new ArrayList<>();
        for (int j = 0; j < this.pollingQueue.size(); j++) {
        FloatPacketType pt = this.pollingQueue[j];
        ids.add(int.valueOf(pt.idOfCommand));
        } 
        return ids;
    }*/
    
    /**
     * write input from robot.cpp
     * TODO: pretty much this is the one that it doesn't use or at least they do the same thing
    */
    
    /* void writeFloats(int id, std::vector<Complex> values) {
        //TODO double check this
        //float[values.size][2]
        float* floatvalues = reinterpret_cast<float(&)[2]>(values); //allocate array of complexes that is an array of floats that is twice the length
        if (getPacket((int)id) == NULL) {
            FloatPacketType pt = FloatPacketType(id, 64);
            for (int i = 0; i < (pt.getDownstream()).size() && i < values.size(); i++){
                pt.getDownstream()[i] = floatvalues[i]; //TODO: Do I just do it directly once I convert it from complex to float?
            }
            addPollingPacket(pt);
            try {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                //Thread.sleep(10L);
            } catch (const std::exception& e) {
                printf("writeFloats thread sleep exception: ");
                printf(e.what());
            } 
        } else {
            for (int j = 0; j < this->pollingQueue.size(); j++) {
                FloatPacketType pt = this->pollingQueue[j];
                if (/*FloatPacketType.class.isInstance(pt) &&*/ /*pt.idOfCommand == id) {
                  /*  for (int i = 0; i < (pt.getDownstream()).size() && i < values.size(); i++){
                        pt.getDownstream()[i] = floatvalues[i]; 
                    }
                    return;
                } 
            } 
        } 
    } */
    
    /*
    void writeBytes(int id, std::vector<unsigned char> values) {
        if (getPacket(int.valueOf(id)) == NULL) {
        BytePacketType pt = new BytePacketType(id, 64); // why 
        //byte your kneecaps
        for (int i = 0; i < (pt.getDownstream()).size() && i < values.size(); i++){
            pt.getDownstream()[i] = (unsigned char)values[i]; //is a unsigned char
        }
        addPollingPacket(pt);
        try {
            Thread.sleep(10L);
        } catch (InterruptedException e) {
            e.printStackTrace();
        } 
        } else {
        for (int j = 0; j < this.pollingQueue.size(); j++) {
            FloatPacketType pt = this.pollingQueue[j];
            if (BytePacketType.class.isInstance(pt))
            if (pt.idOfCommand == id) {
                for (int i = 0; i < (pt.getDownstream()).size() && i < values.size(); i++)
                pt.getDownstream()[i] = Byte.valueOf(values[i]); 
                return;
            }  
        } 
        } 
    }*/
    
    void writeFloats(int id, std::vector<Complex> values) {
        writeFloats(id, values, true);
    }
    
    void writeFloats(int id, std::vector<Complex> values, bool polling) {
        float* floatvalues = reinterpret_cast<float(&)[2]>(values);
        if (getPacket(id) == NULL) {
            FloatPacketType pt = FloatPacketType(id, 64);
        //if (!polling)//TODO: it's not gonna go here
            //pt.oneShotMode(); 
        for (int i = 0; i < (pt.getDownstream()).size() && i < values.size(); i++)
            pt.getDownstream()[i] = floatvalues[i]; 
        addPollingPacket(pt);
        try {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } catch (const std::exception& e) {
            printf("writeFloats thread sleep exception: ");
            printf(e.what());
        } 
        } else {
            for (int j = 0; j < this->pollingQueue.size(); j++) {
                FloatPacketType pt = this->pollingQueue[j];
                if (/*FloatPacketType.class.isInstance(pt) && */pt.idOfCommand == id) {
                    for (int i = 0; i < (pt.getDownstream()).size() && i < values.size(); i++){
                        pt.getDownstream()[i] = floatvalues[i]; 
                    }
                    /*if (!polling){
                        pt.oneShotMode(); 
                    }*/
                    return;
                } 
            }; 
        } 
    }
    
    /*public void writeBytes(int id, Byte[] values) {
        writeBytes(id, values, Boolean.valueOf(true));
    }
    
    public void writeBytes(int id, Byte[] values, Boolean polling) {
        if (getPacket(id) == null) {
        FloatPacketType pt = new BytePacketType(id.intValue(), 64);
        if (!polling.booleanValue())
            pt.oneShotMode(); 
        for (int i = 0; i < (pt.getDownstream()).size() && i < values.size(); i++)
            pt.getDownstream()[i] = Byte.valueOf(values[i].byteValue()); 
        addPollingPacket(pt);
        try {
            Thread.sleep(10L);
        } catch (InterruptedException e) {
            e.printStackTrace();
        } 
        } else {
        for (int j = 0; j < this.pollingQueue.size(); j++) {
            FloatPacketType pt = this.pollingQueue[j];
            if (BytePacketType.class.isInstance(pt))
            if (pt.idOfCommand == id.intValue()) {
                for (int i = 0; i < (pt.getDownstream()).size() && i < values.size(); i++)
                pt.getDownstream()[i] = Byte.valueOf(values[i].byteValue()); 
                if (!polling.booleanValue())
                pt.oneShotMode(); 
                return;
            }  
        } 
        } 
    }*/
    /**
     * reads data given id
     * @param id reportid
     * @return vector of doubles containing read information
     * the one we want to call
    */
    static std::vector<double> readFloats(int id) {
        if (getPacket(id) == NULL) {
            FloatPacketType fl = FloatPacketType(id, 64);
            addPollingPacket(fl);
            try {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            } catch (const std::exception& e) {
                printf("readFloats thread sleep exception: ");
                printf(e.what());
            } 
        } 
        FloatPacketType* pt = getPacket(id);
        std::vector<double> values((pt->getUpstream()).size(), 0);
        for (int i = 0; i < (pt->getUpstream()).size() && i < values.size(); i++){
            values[i] = (double) pt->getUpstream()[i]; 
        }
        return values;
    }
    
    /*public Byte[] readBytes(int id) {
        if (getPacket(id) == null) {
        addPollingPacket(new BytePacketType(id.intValue(), 64));
        try {
            Thread.sleep(10L);
        } catch (InterruptedException e) {
            e.printStackTrace();
        } 
        } 
        FloatPacketType pt = getPacket(id);
        Byte[] values = new Byte[(pt.getUpstream()).size()];
        for (int i = 0; i < (pt.getUpstream()).size() && i < values.size(); i++)
        values[i] = Byte.valueOf(pt.getUpstream()[i].byteValue()); 
        return values;
    }*/

    /**
     * the other readFloats, not supposed to call this one so I'm commenting it out
    */
    /*
    void readFloats(int id, std::vector<double> values) {
        if (getPacket((int)id) == nullptr) {
            addPollingPacket(FloatPacketType(id, 64));
            try {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            } catch (const std::exception& e) {
                printf("readFloats thread sleep exception: ");
                printf(e.what());
            } 
        } 
        for (int j = 0; j < this->pollingQueue.size(); j++) {
            FloatPacketType pt = this->pollingQueue[j];
            //if (FloatPacketType.class.isInstance(pt)){
                if (pt.idOfCommand == id) {
                    for (int i = 0; i < (pt.getUpstream()).size() && i < values.size(); i++) {
                        float d = (float)pt.getUpstream()[i];
                        values[i] = d;
                    } 
                    return;
                }  
            //}
        } 
    }*/
    
    /*
    public void readBytes(int id, byte[] values) {
        if (getPacket(int.valueOf(id)) == null) {
        addPollingPacket(new BytePacketType(id, 64));
        try {
            Thread.sleep(10L);
        } catch (InterruptedException e) {
            e.printStackTrace();
        } 
        } 
        for (int j = 0; j < this.pollingQueue.size(); j++) {
        FloatPacketType pt = this.pollingQueue[j];
        if (BytePacketType.class.isInstance(pt) && 
            pt.idOfCommand == id) {
            for (int i = 0; i < (pt.getUpstream()).size() && i < values.size(); i++)
            values[i] = ((Byte)pt.getUpstream()[i]).byteValue(); 
            return;
        } 
        } 
    }*/

    /**
     * TODO: not sure if this is correct for getting the id from the bytebuffer thing
    */
    static int getId(std::vector<unsigned char> bytes) {
        //ByteBuffer 
        /*unsigned char* bAry = calloc(1024,1); //remember to free

        memcpy(bAry, &length, sizeof(int));    
        memcpy(bAry + sizeof(int), fAry, length*(sizeof(float)));*/

        //looks fancy but I'm guessing I can just do bytes[0]
        //return ByteBuffer.wrap(bytes).order(be).getInt(0); //ByteBuffer given array bytes, 
            //with byte order little endian
            //returns int from buffer's current position
        
        return bytes[0]; //ittle endian reminder
    }
    
    /**
     * actually calls write
    */
    static void process(FloatPacketType packet){
        packet.started = true;
        try {
            if (!isVirtual()) {
                try {
                    std::vector<unsigned char> message = packet.command(packet.idOfCommand, packet.getDownstream());
                    int val = write(message, message.size(), 1); //call to function that will call hid_write
                    if (val > 0) {
                        int readint = read(message, getReadTimeout()); //call to the one that will call hid_read
                        if (readint >= (packet.getUpstream()).size()) {
                            int ID = getId(message); //static method
                            if (ID == packet.idOfCommand) {
                                if (isTimedOut){
                                    printf("Timeout resolved {}", ID); 
                                }
                                isTimedOut = false;
                                std::vector<double> up = packet.parse(message);
                                for (int i = 0; i < (packet.getUpstream()).size(); i++){
                                    packet.getUpstream()[i] = up[i];
                                }
                            } else {
                                for (int i = 0; i < 3; i++){
                                    read(message, getReadTimeout()); 
                                }
                                printf(" ");
                                isTimedOut = true;
                                return;
                            } 
                        } else {
                            isTimedOut = true;
                            return;
                        } 
                    } else {
                        return;
                    } 
                } catch (const std::exception& e) {
                    //t.printStackTrace(System.out);
                    printf("exception: in process under not isvirtual ");
                    printf(e.what());
                    disconnect();
                } 
        } else {
            for (int j = 0; j < (packet.getDownstream()).size() && j < (packet.getUpstream()).size(); j++){
                packet.getUpstream()[j] = packet.getDownstream()[j]; 
            }
        } 
        if (events.find(packet.idOfCommand) == events.end()){
            //Runnable ???
            //TODO not sure how to apply runnable correctly here
            //I mean do I even need this? not sure where it adds any events from
            if (events.find(packet.idOfCommand) != events.end()) {
                for (Runnable e : events[packet.idOfCommand]) {
                    if (&e != nullptr) {
                        try {
                            //e(*this);//TODO: uh oh
                            e();
                        } catch (const std::exception& e) {
                            std::cout << e.what() << std::endl;
                        }
                    }
                }
            }

            /*
            for (Runnable e : this->events.at((int)packet.idOfCommand)) {
                if (&e != nullptr){
                    try {
                        e(*this);
                    } catch (const std::exception& e) {
                        printf("something went wrong inside process");
                        printf("exception: ");
                        printf(e.what());
                        //t.printStackTrace(System.out);
                    }  
                }
            }  */
        }
    } catch (const std::exception& e) {
        printf("exception: ");
        printf(e.what());
    }
        packet.done = true;
    }
    
    static int getReadTimeout() {
        return readTimeout;
    }
    
    /**
     * calls process which calls write
    */
    static bool connect() {
        if (connectDeviceImp()) {
            setVirtual(false);
        } else {
            setVirtual(true);
        } 
        //this->connected = true;
        connected = true;
        
        //TODO: thread troubles may ensue
        std::thread([&]() {
            while (connected) {
                try {
                    for (int i = 0; i < pollingQueue.size(); i++) {
                        FloatPacketType pollingPacket = pollingQueue[i];
                        if (pollingPacket.sendOk())
                            process(pollingPacket);
                    }
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                }
                try {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                } catch (std::exception& e1) {
                    std::cout << e1.what() << std::endl;
                    connected = false;
                }
            }
            disconnectDeviceImp();
            std::cout << "SimplePacketComs disconnect" << std::endl;
        }).detach();
        //Runnable* t1 = new Runnable();
        //t1->start();
        //Runnable r(*this);// {
        /*while (SimpleComsDevice.this.connected) {
            try {
                for (int i = 0; i < SimpleComsDevice.this.pollingQueue.size(); i++) {
                    FloatPacketType pollingPacket = SimpleComsDevice.this.pollingQueue.get(i);
                    if (pollingPacket.sendOk())
                    SimpleComsDevice.this.process(pollingPacket); 
                } 
            } catch (const std::exception& e) {
                printf("connect thread exception: ");
                printf(e.what());
            } 
            try {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            } catch (const std::exception& e1) {
                printf("connect thread sleep exception: ");
                printf(e1.what());
                SimpleComsDevice.this.connected = false;
            } 
        } 

        SimpleComsDevice.this.disconnectDeviceImp();
        printf("SimplePacketComs disconnect");*/
        //}.start();
        return true;
    }

    /**
     * TODO: maybe not how it's supposed to be done but it's better to be safe
    */
    static void disconnect() {
        hid_close(handle);
        hid_exit();
        connected = false;
    }
    
    static bool isVirtual() {
        return virtualv;
    }
    
    static void setVirtual(bool virtualv) {
        virtualv = virtualv;
    }
    
    static void setReadTimeout(int readTimeout) {
        readTimeout = readTimeout;
    }
    
    /*String getName() {
        return this->name;
    }
    
    void setName(String name) {
        this.name = name;
    }*/
    
    static bool getIsTimedOut() {
        return isTimedOut;
    }
    
    //int read(byte[] paramArrayOfbyte, int paramInt);
    
    //int write(byte[] paramArrayOfbyte, int paramInt1, int paramInt2);
    
    //k what does this do?
    //does it actually do the disconnection
    static bool disconnectDeviceImp(){
        //do this just in case
        hid_close(handle);
        hid_exit();
    }
    
    /**
     * TODO: why isn't this implemented
    */
    static bool connectDeviceImp();

    /**
     * setConnected
     * @param connected value 
    */
    static void setConnected(bool set){
        connected = set;
    }

    /**
     * getConnected
     * @return connected value 
    */
    static bool getConnected(){
        return connected;
    }

    /**
     * getPollingQueue
     * @return pollingQueue
    */
    static std::vector<FloatPacketType> getPollingQueue(){
        return pollingQueue;
    }

    /**
     * setPollingQueue
     * @param pollingQueue
    */
    static void setPollingQueue(std::vector<FloatPacketType> pollingQueue){
        pollingQueue = pollingQueue;
    }


    /**
     * write using hidapi
    */
    static int write(std::vector<unsigned char> packet, int len, unsigned char reportID){
        
        try{
            //data
                //java just has reportid being 0 cast to a byte
            //first byte is a thing
            //given input array with 3 values
            //
        /*  packet = zeros(15, 1, 'single'); % creates an empty 15x1 array to write to the robot
                packet(1) = 0; % bypasses interpolation
                packet(3) = array(1); % sets first motor's position value to the first value of array 
                packet(4) = array(2); % sets second motor's position value to the second value of array
                packet(5) = array(3); % sets third motor's position value to the third value of array */
            //size in matlab: self.write(1848, packet)
            if(!validHandle(handle)){
                printf("unable to open device\n");
                hid_exit();
                throw ("unable to open device");
 		        return 1;
            }
            
            //put each packet value in double array
            //TODO: may need to change this to packet size anyway
            /*const int dslen = 3;//packet.size(); //we know it's 3 I'm not dealing with c++ rn
            double ds[dslen]; //now put this in buf somewhere
            for(int i = 0; i < dslen; i++){
                ds[i] = packet[i];
            }*/
            //option A: buf[1] = ds;
            //option B: buf[1] = packet[0];
            //          buf[2] = packet[1];
            //          buf[3] = packet[2];
            //based on the data type of buf it must be b..
            //I'm guessing it'd be index 1, 2, 3? 
            //I cannot be sure cause the matlab code is missing
        
            //data is buf
            /*const int length = 256; //I'd like to think it doesn't actually matter that much what the length is
            unsigned char buf[length + 1];
            //Complex buf[length + 1]; //problem is it needs to be unsigned char
            memset(buf,0,sizeof(buf));
            //in java they just put a byte 0
            buf[0] = reportID; //TODO:OKAY actually this is not reportID, it's a thing in the Packet structure in Java

            //Probably going to change this due to SimpleComsDevice stuff
            //set coordinates
            buf[1] = packet[0];
            buf[2] = packet[1];
            buf[3] = packet[2];*/

            // Precondition checks
            if (packet.size() < len) {
                len = packet.size();
            }

            //int lensend = 4; //TODO: hardcoded this as well, I think this is right

            unsigned char* bytesbuffer;

            bool dropReportIdZero = false;
            if (dropReportIdZero == 0 && reportID == 0) {
                // Use the alternative buffer representation that does
                // not include report ID 0x00
                // This overcomes "The parameter is incorrect" errors on
                // Windows 8 and 10
                // See the commentary on the dropReportIdZero flag for more info
                //report = new WideStringBuffer(len);
                /*if (len >= 1) {
                    System.arraycopy(data, 0, report.buffer, 0, len);
                }*/
                bytesbuffer = &packet[0];
            } else {
                // Put report ID into position 0 and fill out buffer
                //report = new WideStringBuffer(len + 1);
                bytesbuffer[0] = reportID;
                if (len >= 1) {
                    
                    //copy packet into bytesbuffer at index 1
                    for(int i = 0; i < packet.size(); i++){
                        bytesbuffer[i+1] = packet[i];
                    }
                    
                }
            }
            int res = hid_write(handle, bytesbuffer, len); //hid_device *dev, const unsigned char *data, size_t length
            printf("%d", res);
            if(res < 0){
                printf("Unable to write()/2: %ls\n", hid_error(handle));
            }
            return res;
        }
        //TODO: rn the try catch stuff is wrong but I don't know what error to catch for this
        catch (int res) {
            printf("Command error, reading too fast\n");
        }
    }


    /**
     * read using hidapi
     * reads position data from each motor
    */
    static int read(std::vector<unsigned char> bytes, int milliseconds){



        
        //matlab has
        //self.read(1910); 1910 is idOfCommand
        //I THINK 1910 is reportID aka what goes in the first position of the buffer
        if(!validHandle(handle)){
            printf("unable to open device\n");
            hid_exit();
            throw ("unable to open device");
        }

        //data is buf
        /*const int length = 256; //I'd like to think it doesn't actually matter that much what the length is
        unsigned char buf[length + 1];
        memset(buf,0,sizeof(buf));*/
        
        //it wants unsigned char * instead of vector
        unsigned char* bytesbuffer = &bytes[0];
        int res = hid_read_timeout(handle, bytesbuffer, bytes.size(), milliseconds); //hid_device *dev, unsigned char *data, size_t length, int milliseconds
        printf("%d", res);
        
        /*
        //data is buf
        const int length = 256; //I'd like to think it doesn't actually matter that much what the length is
        unsigned char buf[length + 1];
        memset(buf,0,sizeof(buf));
        buf[0] = reportID;
        //I think this is how to get the returned info
        std::vector<double> retbuf;

        //int milliseconds = 500;
        

        //getting handle from above
        int res = hid_read_timeout(handle, buf, length, milliseconds); //hid_device *dev, unsigned char *data, size_t length, int milliseconds
        printf("%d", res);
        if(res < 0){
            printf("Unable to read()/2: %ls\n", hid_error(handle));
            throw("unable to read");
        }

        //coordinate info is buf[1], buf[2], and buf[3]
        for(int i = 1; i < 4; i++){
            retbuf.push_back(buf[i]);
        }*/

        return res;
        //return retbuf; //TODO: I hope it returns the right thing
    }

    /*
    // A Functor
    struct Runnable
    {
        Runnable(SimpleComsDevice a) {  }
    
        // This operator overloading enables calling
        // operator function () on objects of increment
        //how do I get the a
        int operator () ( SimpleComsDevice a) const {
            while (a.getConnected()) {
                try {
                    std::vector<FloatPacketType> pollingQueue = a.getPollingQueue();
                    for (int i = 0; i < pollingQueue.size(); i++) {
                        FloatPacketType pollingPacket = pollingQueue[i];
                        if (pollingPacket.sendOk()){
                            a.process(pollingPacket); //whys this the part I"m struggling with
                        }
                    } 
                } catch (const std::exception& e) {
                    printf("connect thread exception: ");
                    printf(e.what());
                } 
                try {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                } catch (const std::exception& e1) {
                    printf("connect thread sleep exception: ");
                    printf(e1.what());
                    a.setConnected(false);
                } 
            } 
            
            a.disconnectDeviceImp();
            printf("SimplePacketComs disconnect");
            return 0;
        };

    };*/
};

/*
class Runnable : public Thread
{
  public:
    void *run(SimpleComsDevice a) {
       while (a.getConnected()) {
            try {
                std::vector<FloatPacketType> pollingQueue = a.getPollingQueue();
                for (int i = 0; i < pollingQueue.size(); i++) {
                    FloatPacketType pollingPacket = pollingQueue[i];
                    if (pollingPacket.sendOk()){
                        a.process(pollingPacket); //whys this the part I"m struggling with
                    }
                } 
            } catch (const std::exception& e) {
                printf("connect thread exception: ");
                printf(e.what());
            } 
            try {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            } catch (const std::exception& e1) {
                printf("connect thread sleep exception: ");
                printf(e1.what());
                a.setConnected(false);
            } 
        } 
        
        a.disconnectDeviceImp();
        printf("SimplePacketComs disconnect");
        return 0;
    }
};


//#include <pthread.h>
class Thread
{
  public:
    Thread();
    virtual ~Thread();

    int start();
    int join();
    int detach();
    pthread_t self();

    virtual void* run() = 0;

    //Constructor
    Thread::Thread() : m_tid(0), m_running(0), m_detached(0) {}

    //Destructor
    Thread::~Thread()
    {
        if (m_running == 1 && m_detached == 0) {
            pthread_detach(m_tid);
        }
        if (m_running == 1) {
            pthread_cancel(m_tid);
        }
    }

    //run
    static void* runThread(SimpleComsDevice* arg)
    {
        return ((Thread*)arg)->run();
    }

    //run thread
    static void* runThread(void* arg);

    //start
    int Thread::start()
    {
        int result = pthread_create(&m_tid, NULL, runThread, this);
        if (result == 0) {
            m_running = 1;
        }
        return result;
    }

    int Thread::join()
    {
        int result = -1;
        if (m_running == 1) {
            result = pthread_join(m_tid, NULL);
            if (result == 0) {
                m_detached = 1;
            }
        }
        return result;
    }

    int Thread::detach()
    {
        int result = -1;
        if (m_running == 1 && m_detached == 0) {
            result = pthread_detach(m_tid);
            if (result == 0) {
                m_detached = 1;
            }
        }
        return result;
    }

    //get thread id
    pthread_t Thread::self() {
        return m_tid;
    }

  private:
    pthread_t  m_tid;
    int        m_running;
    int        m_detached;



};*/




/*

class Runnable {
    public:
        virtual void* run() = 0;
        virtual ~Runnable() = 0;
};

// Pure virtual destructor: function body required
Runnable::~Runnable(){};*/

//multithreading
/*
class Thread {
    public:
        Thread(std::auto_ptr<Runnable> run);
        Thread();
        virtual ~Thread();
        void start();
        void* join();
    private:
        HANDLE hThread;
        unsigned wThreadID;
        // runnable object will be deleted automatically
        std::auto_ptr<Runnable> runnable;
        Thread(const Thread&);
        const Thread& operator=(const Thread&);
        // called when run() completes
        void setCompleted();
        // stores return value from run()
        void* result;
        virtual void* run() {return 0;}
        static unsigned WINAPI startThreadRunnable(LPVOID pVoid);
        static unsigned WINAPI startThread(LPVOID pVoid);
        void printError(LPTSTR lpszFunction, LPSTR fileName, int lineNumber);
};

Thread::Thread(std::auto_ptr<Runnable> r) : runnable(r) {
	if(!runnable.get())
		printError("Thread(std::auto_ptr<Runnable> r) failed at ",
				__FILE__, __LINE__);
	hThread = 
		(HANDLE)_beginthreadex(NULL,0,Thread::startThreadRunnable,
				(LPVOID)this, CREATE_SUSPENDED, &wThreadID;);
	if(!hThread)
		printError("_beginthreadex failed at ",__FILE__, __LINE__);
}

Thread::Thread() : runnable(NULL) {
	hThread = 
		(HANDLE)_beginthreadex(NULL,0,Thread::startThread,
				(LPVOID)this, CREATE_SUSPENDED, &wThreadID;);
	if(!hThread)
		printError("_beginthreadex failed at ",__FILE__, __LINE__);
}

unsigned WINAPI Thread::startThreadRunnable(LPVOID pVoid) {
	Thread* runnableThread = static_cast<Thread*>(pVoid);
	runnableThread->result = runnableThread->runnable->run();
	runnableThread->setCompleted();
	return reinterpret_cast<unsigned>(runnableThread->result);
}

unsigned WINAPI Thread::startThread(LPVOID pVoid) {
	Thread* aThread = static_cast<Thread*>(pVoid);
	aThread->result = aThread->run();
	aThread->setCompleted();
	return reinterpret_cast<unsigned>(aThread->result);
}

Thread::~Thread() {
	if(wThreadID != GetCurrentThreadId()) {
		DWORD rc = CloseHandle(hThread);
		if(!rc) {
            printError("CloseHandle failed at ",__FILE__, __LINE__);
        }
	}
}

void Thread::start() {
	assert(hThread);
	DWORD rc = ResumeThread(hThread);
	// thread created is in suspended state, 
	// so this starts it running
	if(!rc) printError
			("ResumeThread failed at ",__FILE__, __LINE__);
}

void* Thread::join() {
	// A thread calling T.join() waits until thread T completes.
	return result;
}

void Thread::setCompleted() {
	// Notify any threads that are waiting in join()
}

void Thread::printError(LPSTR lpszFunction, LPSTR fileName, int lineNumber)
{
	TCHAR szBuf[256];
	LPSTR lpErrorBuf;
	DWORD errorCode=GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER||
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		(LPTSTR)&lpErrorBuf;,
		0,
		NULL);
	wsprintf(szBuf, "%s failed at line %d in %s with error %d: %s", 
		     lpszFunction, lineNumber, fileName, errorCode, lpErrorBuf);
	DWORD numWritten; 
	WriteFile(GetStdHandle(STD_ERROR_HANDLE),
		szBuf,
		strlen(reinterpret_cast <const char *> (szBuf)),
		&numWritten;,
		FALSE);
	LocalFree(lpErrorBuf);
	exit(errorCode);
}

class simpleRunnable: public Runnable {
    public:
        simpleRunnable(int ID) : myID(ID) {}
        virtual void* run() {
            std::cout << "Thread " << myID << " is running" << std::endl;
            return reinterpret_cast<void*>(myID);
        }
    private:
        int myID;
};

class simpleThread: public Thread {
    public:
        simpleThread(int ID) : myID(ID) {}
        virtual void* run() {
            std::cout << "Thread " << myID << " is running" << std::endl;
            return reinterpret_cast<void*>(myID);
        }
    private:
        int myID;
};

/*
int main() {
	// thread1 and thread2 are created on the heap
	// thread3 is created on the stack
	// The destructor for thread1 and thread2 will automatically 
	// delete the thread objects.
	std::auto_ptr<Runnable> r(new simpleRunnable(1));
	std::auto_ptr<Thread> thread1(new Thread(r));
	thread1->start();
	std::auto_ptr<simpleThread> thread2(new simpleThread(2));
	thread2->start();
	simpleThread thread3(3);
	thread3.start();
	// wait for the threads to finish
	int result1 = reinterpret_cast<int>(thread1->join());
	int result2 = reinterpret_cast<int>(thread2->join());
	int result3 = reinterpret_cast<int>(thread3.join());
	std::cout << result1 << ' ' << result2 << ' ' << result3 
		<< std::endl;
	return 0;
}*/