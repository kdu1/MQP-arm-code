/*#include "Runnable.h"
//#include "AbstractSimpleComsDevice.h"
/**
 * Quite sure this is functionally similar to Java Runnable
*/
/*
class Runnable : public Thread
{
  public:
    void *run(AbstractSimpleComsDevice a) {
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
};*/