#include <vector>
#include <unordered_map>
//#include "PacketType.h"
#include "FloatPacketType.h"
//#include "Thread.h"
//#include "Runnable.h" //circular

#include <iostream>
#include <memory>
#include <cassert>
//#include <windows.h>
//#include <process.h>
//#include "hidapi.h"
#include <hidapi/hidapi.h>
//#include <libusb.h>
//#include <hidapi_libusb.h>
//#include "hidapi.h"

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
static std::vector<FloatPacketType> pollingQueue;
    
static bool virtualv;

//String name = "SimpleComsDevice";

static int readTimeout = 100;

static bool isTimedOut = false;

//definitions
static bool connected;

//HIDPacketComs extends this, which calls read + write, which calls hidDevice.read+write, 
//which calls HidApi.read + write
//which calls hid_api.read + write
class AbstractSimpleComsDevice {

    public:

    // A Functor
    struct Runnable
    {
        Runnable(AbstractSimpleComsDevice a) {  }
    
        // This operator overloading enables calling
        // operator function () on objects of increment
        //how do I get the a
        int operator () ( AbstractSimpleComsDevice a) const {
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

    };
    

    //device:
    //from test.c
    // Open the device using the VID, PID,
    // and optionally the Serial number.
    ////handle = hid_open(0x4d8, 0x3f, L"12345");
    unsigned short vid = 0x16c0;
    unsigned short pid = 0x0486;
    hid_device * handle = hid_open(vid, pid, NULL); //from hid.c

    bool validHandle(hid_device * handle){
        if (!handle) {
            printf("unable to open device\n");
            hid_exit();
            return 1;
        }
    }
    std::unordered_map<int, std::vector<Runnable>> events;
    
    
    
    
    
    static void addPollingPacket(FloatPacketType packet);
    
    /**
     * given id
     * return FloatPacketType pointer corresponding to it
    */
    static FloatPacketType* getPacket(int ID) ;
    
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
     * why are there so many of them what
    */
   //allocate array of complexes that is an array of floats that is twice the length
    //void writeFloats(int id, std::vector<Complex> values);
    
    
    static void writeFloats(int id, std::vector<Complex> values) ;
    
    static void writeFloats(int id, std::vector<Complex> values, bool polling) ;
    
    
    static std::vector<double> readFloats(int id) ;
    
    //void readFloats(int id, std::vector<double> values) ;
    

    /**
     * TODO: not sure if this is correct for getting the id from the bytebuffer thing
    */
    static int getId(std::vector<unsigned char> bytes);
    
    /**
     * actually calls write
    */
    static void process(FloatPacketType packet);
    
    static int getReadTimeout();
    /**
     * calls process which calls write
    */
    static bool connect();
    
    static void disconnect();
    
    static bool isVirtual();
    
    static void setVirtual(bool virtualv);
    
    static void setReadTimeout(int readTimeout);
    
    /*String getName() {
        return this->name;
    }
    
    void setName(String name) {
        this.name = name;
    }*/
    
    static bool getIsTimedOut();
    
    //int read(byte[] paramArrayOfbyte, int paramInt);
    
    //int write(byte[] paramArrayOfbyte, int paramInt1, int paramInt2);
    
    static bool disconnectDeviceImp();
    
    static bool connectDeviceImp();

    /**
     * setConnected
     * @param connected value 
    */
    static void setConnected(bool set);
    /**
     * getConnected
     * @return connected value 
    */
    static bool getConnected();

    /**
     * getPollingQueue
     * @return pollingQueue
    */
    static std::vector<FloatPacketType> getPollingQueue();

    /**
     * setPollingQueue
     * @param pollingQueue
    */
    static void setPollingQueue(std::vector<FloatPacketType> pollingQueue);


    /**
     * write using hidapi
    */
    static int write(std::vector<unsigned char> packet, int len, unsigned char reportID);
    /**
     * TODO: not sure how to get resulting packets returned
     * read using hidapi
     * reads position data from each motor
    */
    static int read(std::vector<unsigned char> bytes, int milliseconds);

};
