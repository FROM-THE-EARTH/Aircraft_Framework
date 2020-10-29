#include "GPS.h"

GPS::GPS(PinName gpstx,PinName gpsrx): _gps(gpstx,gpsrx)
{
    latitude=0;
    longitude=0;
    
    gpscmd("314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0");
    int odr=ODR;
    odr=1000/odr;
    sprintf(buf,"300,%d,0,0,0,0",odr);
    gpscmd(buf);
    
    int baud=GPSBAUD;
    
    for(int i=0;i<100;i++){
        buf[i]=0;
    }
    
    sprintf(buf,"251,%d",baud);
    
    gpscmd(buf);
    gpscmd(buf);
    
    
    _gps.set_baud(GPSBAUD);
    
    //_gps.attach(this,&GPS::getgps);
}

GPS::~GPS()
{
}

void GPS::gpscmd(char *command)
{
    int len=strlen(command);
    char xordata[len+4];
    sprintf(xordata,"PMTK%s",command);
    
    
    char  xorbuf=0x00;
    for(int i=0;i<len+4;i++){
        xorbuf=xorbuf^xordata[i];
        }
        
    
    char packet[len+8];
    if(xorbuf>0x0f){
        sprintf(packet,"$%s*%x",xordata,xorbuf);
        }
    else {
        sprintf(packet,"$%s*0%x",xordata,xorbuf);
        }
    
    
    /*_gps.printf(packet);
    _gps.putc(0x0D);
    _gps.putc(0x0A);*/
    _gps.write(packet,len + 8);
    char c[1];
    c[0] = 0x0D;
    _gps.write(c, 1);
    c[0] = 0x0A;
    _gps.write(c, 1);
}

void GPS::getgps()
{
    char buf[1];
    _gps.read(buf, 1); 
    if(buf[0] =='$'){
    //if(_gps.getc()=='$'){
        
    int counter;
    char sample=0x00;
    
    int point1=0;
    int point2=0;
        
        
        for(counter=0;counter<18;counter++){
            char buf[1];
            _gps.read(buf, 1); 
         sample=buf[0];  
        }
        
        if(sample=='A'){
            
        //初期化
        latitude=0;
        longitude=0;
        
            //データ取得
            for(counter=0;sample!='\n';counter++){
                char buf[1];
            _gps.read(buf, 1); 
         sample=buf[0];  
              buf[counter]=sample;
              
                if(buf[counter]=='.'&&point1==0){
                  point1=counter;
                }
                
                if(counter>8&&buf[counter]=='.'&&point2==0){
                    point2=counter;
                }
            }
            
            //緯度計算
            for(counter=1;counter<point1-2;counter++){
                
                    float num=1;
                    for(int i=point1-counter-3;i>0;i--){
                        num=num*10;
                    }
                    
                    latitude+=(float)num*(buf[counter]-48);
                }
                
            float minute=0;
            
            minute+=(buf[point1-2]-48)*10;
            minute+=(buf[point1-1]-48);
                       
                for(counter=point1+1;counter<point1+5;counter++){
                    float num=1;
                    
                    for(int i=counter-point1;i>0;i--){
                        num=num*0.1f;
                    }
                    
                    minute+=(float)num*(buf[counter]-48);
                }
            
            latitude+=minute/60;
            
            
            //経度計算
            for(counter=point1+8;counter<point2-2;counter++){
                
                    float num=1;
                    for(int i=point2-counter-3;i>0;i--){
                        num=num*10;
                    }
                    
                    longitude+=(float)num*(buf[counter]-48);
                }
                
            minute=0;
            
            minute+=(buf[point2-2]-48)*10;
            minute+=(buf[point2-1]-48);
                       
                for(counter=point2+1;counter<point2+5;counter++){
                    float num=1;
                    
                    for(int i=counter-point2;i>0;i--){
                        num=num*0.1f;
                    }
                    
                    minute+=(float)num*(buf[counter]-48);
                }
            
            longitude+=minute/60;
            
            
                result=true;
            }
        
            else{
                result=false;
            }
        
        
        
        
        
    }
}