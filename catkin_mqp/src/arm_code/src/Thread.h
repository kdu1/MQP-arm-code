#include <pthread.h> //not supported on windows

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
    Thread::~Thread();

    //run
    static void* runThread(void* arg);

    //run thread
    static void* runThread(void* arg);

    //start
    int Thread::start();

    int Thread::join();

    int Thread::detach();

    //get thread id
    pthread_t Thread::self();
    
  private:
    pthread_t  m_tid;
    int        m_running;
    int        m_detached;



};