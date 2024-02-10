 #include "HC_SR04.h"
    
    HC_SR04::HC_SR04(PinName trigPin, PinName echoPin, Kernel::Clock::duration_u32 updateRate, Kernel::Clock::duration_u32 timeout):_trig(trigPin), _echo(echoPin)
    {
        _updateRate = updateRate;
        _timeout = timeout;
    }
    
    HC_SR04::HC_SR04(PinName trigPin, PinName echoPin, Kernel::Clock::duration_u32 updateRate, Kernel::Clock::duration_u32 timeout, void onUpdate(int))
    :_trig(trigPin), _echo(echoPin)
    {
        _onUpdateMethod = onUpdate;
        _updateRate = updateRate;
        _timeout = timeout;
        _t.start ();
    }
    void HC_SR04::_startT()
    { 
        if(chrono::duration_cast<chrono::milliseconds>(_t.elapsed_time()).count()>600)
        {
            _t.reset ();
        }
        start = chrono::duration_cast<chrono::microseconds>(_t.elapsed_time()).count();
    }
        
    void HC_SR04::_updateDist()
    {
        end = chrono::duration_cast<chrono::microseconds>(_t.elapsed_time()).count();
        done_flag = true;
        _distance = (end - start)/6;       
        _tout.detach();
        _tout.attach(callback(this,&HC_SR04::_startTrig), _updateRate);   
    }
    void HC_SR04::_startTrig(void)
    {
            _tout.detach();
            _trig=1;             
            wait_us(10);        
            done_flag = false;            
            _echo.rise(callback(this,&HC_SR04::_startT));   
            _echo.fall(callback(this,&HC_SR04::_updateDist));
            _echo.enable_irq ();
            _tout.attach(callback(this,&HC_SR04::_startTrig),_timeout);
            _trig=0;                     
    }
    
    int HC_SR04::getCurrentDistance(void)
    {
        return _distance;
    }
    void HC_SR04::pauseUpdates(void)
    {
        _tout.detach();
        _echo.rise(NULL);
        _echo.fall(NULL);
    }
    void HC_SR04::startUpdates(void)
    {
        _startTrig();
    }
    void HC_SR04::attachOnUpdate(void method(int))
    {
        _onUpdateMethod = method;
    }
    void HC_SR04::setUpdateRate(Kernel::Clock::duration_u32 updateRate)
    {
        _updateRate = updateRate;
    }
    bool HC_SR04::isUpdated(void)
    {
        bool temp_flag = done_flag;
        done_flag = false;
        return temp_flag;
    }
    void HC_SR04::checkDistance(void)
    {
        if(isUpdated())
        {
            (*_onUpdateMethod)(_distance);
        }
    }
