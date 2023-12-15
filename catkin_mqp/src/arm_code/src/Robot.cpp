//Robot.cpp
#include <ros/ros.h>
#include <iostream>
#include <vector>
#include <thread>
#include <math.h>
#include "std_msgs/Int32.h"
#include <hidapi/hidapi.h>
//#include <libusb.h>
//#include <hidapi_libusb.h>
//#include "hidapi.h"
//#include <ForwardDeclarations.h>
#include <complex.h>
//#include "rt_nonfinite.h"
//#include "rt_defines.h"
#include <cmath>
#include <valarray>
#include "SimpleComsDevice.h"
#include <sensor_msgs/JointState.h>
#include "ik3001.h"
#include "cubic_traj.h"
typedef std::complex<float> Complex;
typedef std::valarray<Complex> CArray;


//#include "ik3001.h" //should be included hopefully



//#include "FloatPacketType.h"
//#include "PacketType.h"


#define _USE_MATH_DEFINES
# define M_PI           std::complex<float>(3.14159265358979323846, 0)  /* pi */

//also in hid.c
struct hid_device_;
		typedef struct hid_device_ hid_device; /**< opaque hidapi structure */


/**
 * TODO: technically this does not account for the complex component.
*/
bool lessthancomplexreal (Complex l, const float r)
{
    //return l.imag() < r && l.real() < r;
    
    float floatval = reinterpret_cast<float(&)>(l);
    return floatval < r * r;

}

//device:
//from test.c
// Open the device using the VID, PID,
// and optionally the Serial number.


//defining this in SimpleComsDevice instead
/*
unsigned short vid = 0x16c0;
unsigned short pid = 0x0486;
hid_device * handle = hid_open(vid, pid, NULL); //from hid.c*/

/*bool validHandle(hid_device * handle){
    if (!handle) {
        printf("unable to open device\n");
        hid_exit();
        return 1;
    }
}*/

/**
 * constructor
*/
class Robot{
    public:
        CArray endpts = {std::complex<float>(0, 0), std::complex<float>(0,0), std::complex<float>(0,0)};
        int MAX = 3;
        //SimpleComsDevice a;
        ros::Subscriber pos_subscriber_; //gonna be honest not really sure what to subscribe to
        ros::Publisher _servo_jp_publisher;

    /**
     * Robot constructor
     * @param nh - NodeHandle for ros
     * Calls connect method from SimpleComsDevice
    */
    Robot(ros::NodeHandle *nh){
        //SimpleComsDevice a;
        //this->a = a;
        SimpleComsDevice::connect();
        pos_subscriber_ = nh->subscribe(
            "pos_command", 10, &Robot::callbackPosCommand, this);
        _servo_jp_publisher = nh->advertise<sensor_msgs::JointState>("servo_jp", 10);
    };

    void disconnect(){
        SimpleComsDevice::disconnect();
    }

    /**
     * TODO: Callback function get position(?) from ros
    */
    void callbackPosCommand(const sensor_msgs::JointState &msg)
    {
        std::vector<double> pos = msg.position;
        int possize = pos.size();
        CArray carr(possize);
        //get positions and put them in another array then call servo_jp with contents
        for(int i = 0; i < possize; i++){

            printf("{}", pos[i]);
            carr[i] = std::complex<float>(pos[i],0);
        }
        //servo_jp(msg); //commenting out for now

    }

    /**
     * runs a trajectory using coefficients passed in as tc, and a total
     * runtime of t. if s is true, tc was calculated for joint space, if
     * false tc was calculated for task space. Can be run on cubic or
     * quintic trajectories. It also takes in a model to update the model
     * live as the robot moves through the trajectory.
    */
    std::vector<std::vector<Complex>> run_trajectory(bool s, CArray tc, double t) {
        Complex a1, a2, a3; //output to servo
        //number of columns in tc
        //TODO so tc should be 4x1 but that doesn't seem to make sense in this function?
        //int tt = tc[0].size(); 
        //Complex tt = tc[0].size();
        int tt = 4; //TODO: HARDCODING TO GET RID OF ERROR
        //creates empty array to store joint position data
        std::vector<std::vector<Complex>> D(8000, std::vector<Complex>(4));
        std::vector<CArray> jd = measured_js(true, true);
        CArray jp = jd[0]; //matlab is 0 indexed!! pretty sure this is the same as jd(1, :)?
        CArray jv = jd[1]; //TODO: 2d or 3d?
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); //pause(10)
        jp[3] = std::complex<float>(0, 0); //TODO: I don't know why it's 2 dimensions here
        //setting rows 0-3 to jp, 4-6 to jv TODO double check this
        D[0][0] = jp[0];
        D[0][1] = jp[1];
        D[0][2] = jp[2];
        D[0][3] = jp[3];
        D[0][4] = jv[0];
        D[0][5] = jv[1];
        D[0][6] = jv[2];
        int i = 2;
        
        //tic
        auto start = std::chrono::high_resolution_clock::now();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Elapsed time: " << elapsed.count() << " s\n";
        double currTLoop = elapsed.count();
        //TODO: not sure if elapsed will be correct for the loop
        while (currTLoop < t) {
            
            std::vector<CArray> detCheckGet = this->measured_js(true, false); //2 rows by 3 cols
            CArray detCheck = detCheckGet[0]; //row 1 ?
            std::vector<std::vector<Complex>> jacobv = this->jacob3001(detCheck.apply(std::conj)); //transpose 3 by 1
            jacobv = {jacobv[0], jacobv[1], jacobv[2]};
            Complex DetJv = det(jacobv);
            // checks for when the arm is close to reaching singularity
            Complex diff = DetJv - std::complex<float>(1.1, 0);
            if (lessthancomplexreal(DetJv, 1.1)) {
                break;
            }
            //update clock
            end = std::chrono::high_resolution_clock::now();
            elapsed = end - start;
            Complex curT = elapsed.count();
           
            //TODO: not sure if it maybe be that the next calls to aset actually replace the 2nd and 3rd row respectively but I'm going with this for now
            //tc = {{tc}, {tc}, {tc}};
            //I'm guessing 4 is cubic, 6 is quintic?
            switch (tt) {
                case 4: 
                    a1 = tc[0] + tc[1] * curT + tc[2] * curT * curT + tc[3] * curT * curT * curT;
                    a2 = tc[0] + tc[1] * curT + tc[2] * curT * curT + tc[3] * curT * curT * curT;
                    a3 = tc[0] + tc[1] * curT + tc[2] * curT * curT + tc[3] * curT * curT * curT;
                    break;
                case 6:
                    //a1 = tc[0][0] + tc[0][1] * curT + tc[0][2] * curT * curT + tc[0][3] * curT * curT * curT + tc[0][4] * curT * curT * curT * curT + tc[0][5] * curT * curT * curT * curT * curT;
                    //a2 = tc[1][0] + tc[1][1] * curT + tc[1][2] * curT * curT + tc[1][3] * curT * curT * curT + tc[1][4] * curT * curT * curT * curT + tc[1][5] * curT * curT * curT * curT * curT;
                    //a3 = tc[2][0] + tc[2][1] * curT + tc[2][2] * curT * curT + tc[2][3] * curT * curT * curT + tc[2][4] * curT * curT * curT * curT + tc[2][5] * curT * curT * curT * curT * curT;
                    break;
            }

            //the example I'm using creates the publishers under joint and task space, same format of calling servojp
            //joint space
            if (s == true) {
                this->servo_jp({a1, a2, a3});
            }
            //task space
            else {
                //TODO: also seems to require there be an extra TransformStamped message be sent for the task space. Is the ik3001 enough for that? Or do we still need to do that
                //A = self.ik30();
                Complex endpts [3] = {a1, a2, a3};
                std::vector<Complex> A = ik3001(endpts); //TODO: just have this be a CArray
                CArray cA;
                for(int i = 0; i < 3; i++){
                    cA[i] = A[i];
                }
                CArray conttrans = cA.apply(std::conj); 
                /*std::vector<Complex> conttransret;
                //convert it back
                for(int i = 0; i < 3; i++){
                    conttransret[i] = conttrans[i];
                }*/
                this->servo_jp(conttrans);
            }
            /*else {
            std::vector<Complex> A = self.ik3001({a1, a2, a3});
            self.servo_jp({A[0], A[1], A[2]});
            }*/
    
            std::vector<CArray> jd = this->measured_js(true, true);
            CArray jp = jd[0];
            CArray jv = jd[1];
            jp[3] = curT; //TODO: I am once again not sure whether jp is 1d or 2d
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            D[i][0] = jp[0];
            D[i][1] = jp[1];
            D[i][2] = jp[2];
            D[i][3] = jp[3];
            D[i][4] = jv[0];
            D[i][5] = jv[1];
            D[i][6] = jv[2];
            D[i][7] = DetJv; // records determinate of top 3x3 of Jacobian
            i = i + 1;
            
            //update clock again for while loop
            end = std::chrono::high_resolution_clock::now();
            elapsed = end - start;
            currTLoop = elapsed.count();
        }
        
        D.resize(i-1, std::vector<Complex>(4)); // cuts D to size and returns it
        return D; 
    };


    /**
     * write converting it to the correct packet thing format
    */
   /**
    void writeFloats(int id, std::vector<Complex> values) {
        writeFloats(id, values, true);
    }
    

    //okay so there's a connect method that calls a process method that calls the actual write using
    //the pollingpacket 
    void writeFloats(int id, std::vector<Complex> values, bool polling) {
        if (getPacket(id) == NULL) {
            FloatPacketType pt = FloatPacketType(id, 64);
            if (!polling.booleanValue()){
                pt.oneShotMode(); 
            }
            //why does the thing not have braces??
            for (int i = 0; i < (pt.getDownstream()).size() && i < values.size(); i++){
                pt.getDownstream()[i] = values[i];
                addPollingPacket(pt);
            }
            try {
                Thread.sleep(10L);
            } catch (const std::exception& e) {
                printf("writeFloats thread sleep exception: ");
                printf(e.what());
            } 
        } else {
            for (int j = 0; j < this->pollingQueue.size(); j++) {
                PacketType pt = this->pollingQueue.get(j);
                if (FloatPacketType.class.isInstance(pt) && 
                    pt.idOfCommand == id) {
                    for (int i = 0; i < (pt.getDownstream()).length && i < values.size(); i++){
                        pt.getDownstream()[i] = (float)values[i]; 
                    }
                    if (!polling.booleanValue()){
                        pt.oneShotMode(); 
                    }
                    return;
                } 
            } 
        } 
    }*/

    /**
     * writes information, calls writeFloats method from SimpleComsDevice
     * @param reportID, values the id of the info to write, pretty sure it's the id in the first byte of the information for calling hid_write, and the values to write
    */
    void write(int id, std::vector<Complex> values){
        
        try{
            SimpleComsDevice::writeFloats(id, values);//Then convert it to floats cause the complex is just floats with twice as big array
            
        }
        catch (const std::exception& e) {
            printf("Command error, reading too fast\n");
        }
    }

 /**
   * Write an Output report to a HID device using a simplified interface
   *
   * <b>HID API notes</b>
   *
   * In USB HID the first byte of the data packet must contain the Report ID.
   * For devices which only support a single report, this must be set to 0x00.
   * The remaining bytes contain the report data. Since the Report ID is mandatory,
   * calls to <code>hid_write()</code> will always contain one more byte than the report
   * contains.
   *
   * For example, if a HID report is 16 bytes long, 17 bytes must be passed to <code>hid_write()</code>,
   * the Report ID (or 0x00, for devices with a single report), followed by the report data (16 bytes).
   * In this example, the length passed in would be 17.
   *
   * <code>hid_write()</code> will send the data on the first OUT endpoint, if one exists.
   * If it does not, it will send the data through the Control Endpoint (Endpoint 0)
   *
   * @param device   The device
   * @param data     The report data to write (should not include the Report ID)
   * @param len      The length of the report data (should not include the Report ID)
   * @param reportId The report ID (or (byte) 0x00)
   *
   * @return The number of bytes written, or -1 if an error occurs
   */

    /**
     * reads information, calls readFloats method from SimpleComsDevice
     * @param reportID he report ID (or (byte) 0x00)
     * @return The number of bytes written, or -1 if an error occurs
    */
    std::vector<double> read(unsigned char reportID){

        //matlab has
        //self.read(1910); 1910 is idOfCommand
        
        //check if connected to device correctly
        /*if(!validHandle(handle)){
            printf("unable to open device\n");
            hid_exit();
            throw ("unable to open device");
        }*/
        
        return SimpleComsDevice::readFloats(reportID);
        //data is buf
        /*const int length = 256;
        unsigned char buf[length + 1];
        memset(buf,0,sizeof(buf));
        buf[0] = reportID;
        //I think this is how to get the returned info
        std::vector<Complex> retbuf;

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
        }

        return retbuf; 
        */
   }

    /**
     * moves the motors to the positions specified by the input array,
     * without interpolation
     * TODO: need to set position limits
     * */
    void servo_jp(CArray array) {
        std::vector<Complex> packet(15, std::complex<float>(0.0,0)); // creates an empty 15x1 array to write to the robot
        packet[0] = 0.0; // bypasses interpolation
        packet[2] = array[0]; // sets first motor's position value to the first value of array 
        packet[3] = array[1]; // sets second motor's position value to the second value of array
        packet[4] = array[2]; // sets third motor's position value to the third value of array
        
        //almost certain this needs to be published to ROS as a JointState
        //Questions:
        //do I publish packet? or I think I can just publish array
        //do I also need to publish endpts?
        //can I just publish it from here? 
        //Do I need to publish it continuously? Would it be better to have a new file for the ros stuff?

        //alternatively, should I just publish the endpts continuously?

        //publishing array data to ROS
        sensor_msgs::JointState msg;
        msg.name ={"x", "y", "z"};
        
        //TODO: how to set limits?
        for(int i=0; i < array.size(); i++){
            msg.position.push_back(reinterpret_cast<float(&)>(array[i]));
            //1 will need to convert the complex to float64 - hope there will be no issue with that
            //2 is this sufficient without velocity and effort?
        }
        
        _servo_jp_publisher.publish(msg);

       
        write(1848, packet); // sends the desired motion command to the robot
        endpts = array; // sets the Robot's endpoint as the endpoint specified by the input array
    }

    /**
     * calculates 6x3 manipulator Jacobian from 3x1 array of joint angles
    */
   
    std::vector<std::vector<Complex>> jacob3001(CArray ja) {
        Complex t1 = ja[0];
        Complex  t2 = ja[1];
        Complex  t3 = ja[2];
        //hardcoded Jacobian calculation
        std::vector<std::vector<Complex>> J = {{(std::complex<float>(5,0)*M_PI*sin((M_PI*t1)/std::complex<float>(180, 0))*sin((M_PI*(t2 - std::complex<float>(90,0)))/std::complex<float>(180, 0))*sin((M_PI*(t3 + std::complex<float>(90,0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0) - (std::complex<float>(5,0)*M_PI*std::complex<float>(sin((M_PI*t1)/std::complex<float>(180, 0)))*std::complex<float>(cos((M_PI*(t2 - std::complex<float>(90,0)))/std::complex<float>(180, 0)))*std::complex<float>(cos((M_PI*(t3 + std::complex<float>(90,0)))/std::complex<float>(180, 0))))/std::complex<float>(9,0) - (std::complex<float>(5,0)*M_PI*sin((M_PI*t1)/std::complex<float>(180, 0))*cos((M_PI*(t2 - std::complex<float>(90,0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0), 
                                                            -(std::complex<float>(5,0)*M_PI*cos((M_PI*t1)/std::complex<float>(180, 0))*sin((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0) - (std::complex<float>(5,0)*M_PI*cos((M_PI*t1)/std::complex<float>(180, 0))*cos((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0))*sin((M_PI*(t3 + std::complex<float>(90,0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0) - (std::complex<float>(5,0)*M_PI*cos((M_PI*t1)/std::complex<float>(180, 0))*cos((M_PI*(t3 + std::complex<float>(90,0)))/std::complex<float>(180, 0))*sin((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0), 
                                                            -(std::complex<float>(5,0)*M_PI*cos((M_PI*t1)/std::complex<float>(180, 0))*cos((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0))*sin((M_PI*(t3 + std::complex<float>(90,0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0) - (std::complex<float>(5,0)*M_PI*cos((M_PI*t1)/std::complex<float>(180, 0))*cos((M_PI*(t3 + std::complex<float>(90,0)))/std::complex<float>(180, 0))*sin((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0)},
                                                              
                                                              {(std::complex<float>(5,0)*M_PI*cos((M_PI*t1)/std::complex<float>(180, 0))*cos((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0) + (std::complex<float>(5,0)*M_PI*cos((M_PI*t1)/std::complex<float>(180, 0))*cos((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0))*cos((M_PI*(t3 + std::complex<float>(90,0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0) - (std::complex<float>(5,0)*M_PI*cos((M_PI*t1)/std::complex<float>(180, 0))*sin((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0))*sin((M_PI*(t3 + std::complex<float>(90,0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0), 
                                                              -(std::complex<float>(5,0)*M_PI*sin((M_PI*t1)/std::complex<float>(180, 0))*sin((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0) - (std::complex<float>(5,0)*M_PI*sin((M_PI*t1)/std::complex<float>(180, 0))*cos((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0))*sin((M_PI*(t3 + std::complex<float>(90,0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0) - (std::complex<float>(5,0)*M_PI*sin((M_PI*t1)/std::complex<float>(180, 0))*cos((M_PI*(t3 + std::complex<float>(90,0)))/std::complex<float>(180, 0))*sin((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0), 
                                                              -(std::complex<float>(5,0)*M_PI*sin((M_PI*t1)/std::complex<float>(180, 0))*cos((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0))*sin((M_PI*(t3 + std::complex<float>(90,0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0) - (std::complex<float>(5,0)*M_PI*sin((M_PI*t1)/std::complex<float>(180, 0))*cos((M_PI*(t3 + std::complex<float>(90,0)))/std::complex<float>(180, 0))*sin((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0)},
                                                              
                                                              {std::complex<float>(0,0), (std::complex<float>(5,0)*M_PI*sin((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0))*sin((M_PI*(t3 + std::complex<float>(90,0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0) - (std::complex<float>(5,0)*M_PI*cos((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0))*cos((M_PI*(t3 + std::complex<float>(90,0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0) - (std::complex<float>(5,0)*M_PI*cos((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0), (std::complex<float>(5,0)*M_PI*sin((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0))*sin((M_PI*(t3 + std::complex<float>(90,0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0) - (std::complex<float>(5,0)*M_PI*cos((M_PI*(t2 - std::complex<float>(90, 0)))/std::complex<float>(180, 0))*cos((M_PI*(t3 + std::complex<float>(90,0)))/std::complex<float>(180, 0)))/std::complex<float>(9,0)},
                                                              {std::complex<float>(0,0), -sin((M_PI*t1)/std::complex<float>(180, 0)), -sin((M_PI*t1)/std::complex<float>(180, 0))},
                                                              {std::complex<float>(0,0), cos((M_PI*t1)/std::complex<float>(180, 0)), cos((M_PI*t1)/std::complex<float>(180, 0))},
                                                              {std::complex<float>(1,0), std::complex<float>(0,0), std::complex<float>(0,0)}};

        return J;
    }

        
        /**
         * TODO: okay this looks wrong and now I'm worried the other generated stuff is wrong
         * calculates 6x3 manipulator Jacobian from 3x1 array of joint angles
        */
       /*
        double* jacob3001(const double ja[3])
        {
        double J_tmp;
        double J_tmp_tmp;
        double b_J_tmp;
        double b_J_tmp_tmp;
        double c_J_tmp;
        double c_J_tmp_tmp;
        double d_J_tmp;
        double d_J_tmp_tmp;
        double e_J_tmp;
        double e_J_tmp_tmp;
        double f_J_tmp;
        double g_J_tmp;
        double h_J_tmp;
        //  calculates 6x3 manipulator Jacobian from 3x1 array of joint
        //  angles
        //  hardcoded Jacobian calculation
        J_tmp_tmp = 3.1415926535897931 * (ja[1] - std::complex<float>(90,0).0) / 180.0;
        J_tmp = std::sin(J_tmp_tmp);
        J_tmp_tmp = std::cos(J_tmp_tmp);
        b_J_tmp_tmp = 3.1415926535897931 * (ja[2] + 90.0) / 180.0;
        b_J_tmp = std::sin(b_J_tmp_tmp);
        c_J_tmp = std::cos(b_J_tmp_tmp);
        b_J_tmp_tmp = 3.1415926535897931 * ja[0] / 180.0;
        c_J_tmp_tmp = std::sin(b_J_tmp_tmp);
        b_J_tmp_tmp = std::cos(b_J_tmp_tmp);
        d_J_tmp_tmp = 15.707963267948966 * c_J_tmp_tmp;
        d_J_tmp = d_J_tmp_tmp * J_tmp_tmp;
        e_J_tmp = 15.707963267948966 * c_J_tmp_tmp * J_tmp;
        J[0] = (e_J_tmp * b_J_tmp / 9.0 - d_J_tmp * c_J_tmp / 9.0) - d_J_tmp / 9.0;
        f_J_tmp = 15.707963267948966 * b_J_tmp_tmp;
        e_J_tmp_tmp = f_J_tmp * J_tmp_tmp;
        g_J_tmp = e_J_tmp_tmp * b_J_tmp;
        f_J_tmp = f_J_tmp * c_J_tmp * J_tmp / 9.0;
        h_J_tmp = 15.707963267948966 * b_J_tmp_tmp * J_tmp;
        J[6] = (-h_J_tmp / 9.0 - g_J_tmp / 9.0) - f_J_tmp;
        J[12] = -g_J_tmp / 9.0 - f_J_tmp;
        J[1] = (e_J_tmp_tmp / 9.0 + e_J_tmp_tmp * c_J_tmp / 9.0) -
                h_J_tmp * b_J_tmp / 9.0;
        d_J_tmp *= b_J_tmp;
        f_J_tmp = d_J_tmp_tmp * c_J_tmp * J_tmp / 9.0;
        J[7] = (-e_J_tmp / 9.0 - d_J_tmp / 9.0) - f_J_tmp;
        J[13] = -d_J_tmp / 9.0 - f_J_tmp;
        J[2] = 0.0;
        J_tmp = 15.707963267948966 * J_tmp * b_J_tmp / 9.0 -
                15.707963267948966 * J_tmp_tmp * c_J_tmp / 9.0;
        J[8] = J_tmp - 15.707963267948966 * J_tmp_tmp / 9.0;
        J[14] = J_tmp;
        J[3] = 0.0;
        J[9] = -c_J_tmp_tmp;
        J[15] = -c_J_tmp_tmp;
        J[4] = 0.0;
        J[10] = b_J_tmp_tmp;
        J[16] = b_J_tmp_tmp;
        J[5] = 1.0;
        J[11] = 0.0;
        J[17] = 0.0;

        return J;
        }*/


    /**
     * returns the position and/or velocity values of the motors in a
     * 2x3 array. takes two booleans to represent whether to return the
     * positions, velocity, or both
    */
    std::vector<CArray> measured_js(bool GETPOS, bool GETVEL) {
        //std::vector<std::vector<Complex>> returnArray(2, std::vector<Complex>(3, std::complex<float>(0.0, 0))); // creates a 2x3 return array
        //position
        std::vector<double> posPacket = read(1910);
        CArray returnComplex1;
        CArray returnComplex2;

        if (GETPOS) { // if GETPOS is true
            Complex pos1 = std::complex<float>((float)posPacket[2], 0); // gets the position of the first motor
            Complex pos2 = std::complex<float>((float)posPacket[4], 0); // gets the position of the second motor
            Complex pos3 = std::complex<float>((float)posPacket[6], 0); // gets the position of the third motor
            //returnArray[0] = {pos1, pos2, pos3}; // sets the top row of the return array to the position values
            std::vector<Complex> returnArray1 = {pos1, pos2, pos3};
            CArray returnComplex1new ( &(returnArray1[0]), returnArray1.size() );
            returnComplex1 = returnComplex1new;
        }
        //velocity
        std::vector<double> velPacket = read(1822); //TODO: reads the position data from each motor
        if (GETVEL) { // if GETVEL is true
            Complex vel1 = std::complex<float>((float)velPacket[2], 0); // gets the velocity of the first motor
            Complex vel2 = std::complex<float>((float)velPacket[5], 0); // gets the velocity of the second motor
            Complex vel3 = std::complex<float>((float)velPacket[8], 0); // gets the velocity of the third motor
            //returnArray[1] = {vel1, vel2, vel3}; // sets the bottom row of the return array to the velocity values
            std::vector<Complex> returnArray2 = {vel1, vel2, vel3};
            CArray returnComplex2new ( &(returnArray2[0]), returnArray2.size() );
            returnComplex2 = returnComplex2new;
        }

        std::vector<CArray> returnComplex = {returnComplex1, returnComplex2};
        return returnComplex; // returns the array of position and velocity values
    }

    /**
     * returns the determinant
    */
    Complex det(std::vector<std::vector<Complex>> input) {
        int n = input[0].size(); //TODO: not sure if always square
        
        Complex d= std::complex<float>(0,0);
        int p, h, k, i, j;
        std::vector<std::vector<Complex>> temp(n, std::vector<Complex>(n));

        if(n==1) {
            return input[0][0];
        } else if(n==2) {
            d=(input[0][0]*input[1][1]-input[0][1]*input[1][0]);
            return d;
        } else {
            for(p=0;p<n;p++) {
            h = 0;
            k = 0;
            for(i=1;i<n;i++) {
                for( j=0;j<n;j++) {
                if(j==p) {
                    continue;
                }
                temp[h][k] = input[i][j];
                k++;
                if(k==n-1) {
                    h++;
                    k = 0;
                }
                }
            }
            d=d+input[0][p]*std::complex<float>(pow(-1,p),0)*det(temp);
            }
            return d;
        }
    }

    /**
     * completes pick and place operation
     * @param xi, yi, zi, color - the location to navigate to
    */
    void pickAndPlace(double xi, double yi, double zi, int color) {
        //Traj_Planner traj_planner(this);
        double traj_time = 3.0;
        double tj2 = 1.0;
        //std::vector<Complex> zero = {std::complex<float>(0,0), std::complex<float>(0,0), std::complex<float>(0,0)};
        //I'm not up for solving the Unsolved Mysteries right now
        //is this 2D? it has to be 2d

        
        CArray aset = cubic_traj(traj_time, {0, 0, 0}, {0, 0, 0}, {100,0,195}, {xi,yi,(zi+30)}).apply(std::conj); 
        std::vector<std::vector<Complex>> D1 = run_trajectory(false, aset, traj_time);
        aset = cubic_traj(traj_time, {0, 0, 0}, {0, 0, 0}, {xi,yi,zi+30}, {xi+7,yi,zi}).apply(std::conj);
        D1 = run_trajectory(false, aset, traj_time);
        //closeGripper(); //TODO: not using gripper so I'm guessing can comment it out
        std::this_thread::sleep_for(std::chrono::seconds(1));
        aset = cubic_traj(tj2, {0, 0, 0}, {0, 0, 0}, {xi,yi,zi}, {100,0,195}).apply(std::conj);
        D1 = run_trajectory(false, aset, tj2);
        // if color == 1
        // if red
        aset = cubic_traj(traj_time, {0,0,0}, {0,0,0}, {100,0,195}, {10,150,30}).apply(std::conj);
        D1 = run_trajectory(false, aset, traj_time);
    }

    //need to exit
    void stop(){
        SimpleComsDevice::disconnectDeviceImp();
    }

};

/*
class Traj_Planner{
    public:
        Robot robot;

        Traj_Planner(Robot robot){
            robot = robot;
        }
};
*/


/**
 * Calls the procedure for moving the arm
*/
/*int mainArmCall(){
    //do the hw5 stuff here

    std::vector<std::vector<int>> cRed(3, std::vector<int>(3, 0));
    std::vector<int> desPos = cRed[0];
    SimpleComsDevice a;
    Robot robot(a);
    int x = desPos[0];
    int y = desPos[1];
    int z = desPos[2];
    robot.servo_jp({std::complex<float>(0), std::complex<float>(0), std::complex<float>(0)});
    std::this_thread::sleep_for(std::chrono::seconds(1));
    robot.pickAndPlace(x, y, z, 1);
    
    return 0;
};*/

/**
 * TODO:main ROS function
*/
int main(int argc, char **argv)
{
    ros::init(argc, argv, "ros_robot");
    ros::NodeHandle nh;

    //ros::AsyncSpinner spinner(4);
    //spinner.start();

    
    //do the hw5 stuff here
    //no idea if it works like this
    std::vector<std::vector<int>> cRed(3, std::vector<int>(3, 0));
    std::vector<int> desPos = cRed[0];
    //SimpleComsDevice a;
    //init robot
    Robot robot(&nh);
    ROS_INFO("ROS robot is now started");

    //move arm
    int x = desPos[0];
    int y = desPos[1];
    int z = desPos[2];
    CArray in = {std::complex<float>(0,0), std::complex<float>(0,0), std::complex<float>(0,0)};
    robot.servo_jp(in);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //robot.pickAndPlace(x, y, z, 1); //test simple servo jp before doing this

    ros::spinOnce();
    
    robot.disconnect();
    //ros::waitForShutdown();

    //one thing I don't really understand is how write works while we're doing both write and sending information through ros
    //an example I found had both
    //but does it mean we don't need to subscribe to anything?
}