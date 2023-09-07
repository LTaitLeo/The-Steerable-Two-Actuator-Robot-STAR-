#include <PID_v1.h> 

#define IN3 5  //define IN3 as pin5
#define IN4 4  //define IN4 as pin4
#define  ENB  9 //define ENB as pin9 (pwm pin)

#define IN1 6  //define IN1 as pin6
#define IN2 7  //define IN2 as pin7
#define  ENA  10 //define ENB as pin10 (pwm pin)


//bluetooth variables
char ble_val;
String My_state="stop";
int My_dir=0;
String speed_follow="NO";

//encoder variables
int motor_A = 2;//interrupt pin is pin2
int motor_B = 3;//interrupt pin is pin3
volatile int count = 0;//increace 1 for every count when forward; decrease 1 for every count when reverse.
volatile int pos = 0;

int motor_A1 = 21;//interrupt pin is pin21
int motor_B1 = 20;//interrupt pin is pin20
volatile int count1 = 0;//increace 1 for every count when forward; decrease 1 for every count when reverse.
volatile int pos1 = 0;


double pwm_diff=0;

//encoder 1
int reducation = 56;//reduction ratio
int pulse = 11; //The number of pulses produced by one revolution of the encoder
int per_round = pulse * reducation * 4;//total pulse per round
long start_time = millis();//The beginning of a calculation cycle for velocity
long interval_time = 50;//One calculation cycle is 50ms
double current_vel;
double angle;

//encoder 2
int reducation1 = 56;
int pulse1 = 11; 
int per_round1 = pulse1 * reducation1 * 4;
long start_time1 = millis();
long interval_time1 = 50;
double current_vel1;
double angle1;


// count position for motor 1
void count_A(){
  //Double frequency
  //output: pulse number*reduction ratio*2
  if(digitalRead(motor_A) == HIGH){

    if(digitalRead(motor_B) == HIGH){//A HIGH B HIGH
      count++;  
      pos++;
    } else {//A HIGH B LOW
      count--;  
      pos--;
    }


  } else {

    if(digitalRead(motor_B) == LOW){//A LOW B LOW
      count++;  
      pos++;
    } else {//A LOW B HIGH
      count--;  
      pos--;
    }  

  }

}

//similar with count_A
//Quadruple frequency
//output: pulse number*reduction ratio*4
void count_B(){
  if(digitalRead(motor_B) == HIGH){

    if(digitalRead(motor_A) == LOW){//B HIGH A LOW
      count++;
      pos++;
    } else {//B HIGH A HIGH
      count--;
      pos--;
    }


  } else {

    if(digitalRead(motor_A) == HIGH){//B LOW A HIGH
      count++;
      pos++;
    } else {//B LOW A LOW
      count--;
      pos--;
    }

  }

}

//get current velocity
void get_current_vel(){
  long right_now = millis();  
  long past_time = right_now - start_time;//calculate past time
  if(past_time >= interval_time){//if past time lager than the calculation cycle
    //1.stop interruption
    noInterrupts();
    //2.calculate velocity
    current_vel = (double)count / per_round / past_time * 1000 * 60;
    //3.reset count
    count = 0;
    //4.reset timer
    start_time = right_now;
    //5.start interruption
    interrupts();

    if(My_dir==0){current_vel=-1*current_vel;} //clockwise
    else if(My_dir==1){current_vel=current_vel;}//counterclockwise
    else if(My_dir==2){current_vel=-1*current_vel;}//clockwise
    
    Serial.print(current_vel);
    Serial.print(",");

  }
}

// count position for motor 2
void count_A1(){


  if(digitalRead(motor_A1) == HIGH){

    if(digitalRead(motor_B1) == HIGH){
      count1++;
      pos1++;  
    } else {
      count1--;  
      pos1--;
    }


  } else {

    if(digitalRead(motor_B1) == LOW){
      count1++; 
      pos1++; 
    } else {
      count1--;  
      pos1--;
    }  

  }

}


void count_B1(){
  if(digitalRead(motor_B1) == HIGH){

    if(digitalRead(motor_A1) == LOW){
      count1++;
      pos1++;
    } else {
      count1--;
      pos1--;
    }


  } else {

    if(digitalRead(motor_A1) == HIGH){
      count1++;
      pos1++;
    } else {
      count1--;
      pos1--;
    }

  }

}


void get_current_vel1(){
  long right_now = millis();  
  long past_time = right_now - start_time1;
  if(past_time >= interval_time1){
   
    noInterrupts();
    
    current_vel1 = (double)count1 / per_round1 / past_time * 1000 * 60;
    
    count1 = 0;
    
    start_time1 = right_now;
    
    interrupts();

    if(My_dir==0){current_vel1=current_vel1;}
    else if(My_dir==1){current_vel1=-1*current_vel1;}
    else if(My_dir==2){current_vel1=-1*current_vel1;}

//    current_vel1=-1*current_vel1;

    Serial.println(current_vel1);

  }
}

//get current position for motor 1
void get_current_pos(){
    angle=(float)pos/2464*360;
     if(angle>360)
  {
    angle=angle-360;
    pos=0;
    }
   else if(angle<-360)
   {
    angle=angle+360;
    pos=0;
    }   
//    Serial.println(angle);

  }

//get current position for motor 2
void get_current_pos1(){
    angle1=(float)pos1/2464*360;
     if(angle1>360)
  {
    angle1=angle1-360;
    pos1=0;
    }
   else if(angle1<-360)
   {
    angle1=angle1+360;
    pos1=0;
    }   
//    Serial.println(angle1);

  }

//-------------------------------------PID-------------------------------------------
//creat PID for motor 1 
//1.current velocity 2.desired pwm 3.target velocity 4.kp 5.ki 6.kd 7.control direction
double pwm;//pwm value for motor drive
double target = 0;//initial velocity
double kp=0.5, ki=1.5, kd=0.1;// PID parameters need to be adjusted
uint8_t L=LOW;
uint8_t H=HIGH;
PID pid(&current_vel,&pwm,&target,kp,ki,kd,DIRECT);

//update velocity for motor 1
void update_vel(){
  //get current velocity
  get_current_vel();
  pid.Compute();//calculate desired pwm value
  digitalWrite(IN3,H);  //control motor drive direction
  digitalWrite(IN4,L);
  analogWrite(ENB,pwm); //control motor velocity

//  Serial.println(pwm);

}


//-------------------------------------PID-------------------------------------------
//creat PID for motor 2
//1.current velocity 2.desired pwm 3.target velocity 4.kp 5.ki 6.kd 7.control direction
double pwm1;
double target1 = 0;
double kp1=0.5, ki1=1.5, kd1=0.075;
uint8_t L1=LOW;
uint8_t H1=HIGH;
PID pid1(&current_vel1,&pwm1,&target1,kp1,ki1,kd1,DIRECT);


void update_vel1(){
  
  get_current_vel1();
  pid1.Compute();
  digitalWrite(IN1,L1);  
  digitalWrite(IN2,H1);
  analogWrite(ENA,pwm1+pwm_diff); 


  if(speed_follow=="YES"){
      //velocity compensation for difference between two motors.
//      if(current_vel1<current_vel){pwm_diff=pwm_diff+0.05;}
//      else if(current_vel1==current_vel){pwm_diff=0;} 
//      else if(current_vel1>current_vel){pwm_diff=pwm_diff-0.05;}
  }

//  Serial.println(pwm_diff);

//  Serial.println(pwm);

}

void setup()
{  
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENB,OUTPUT);

  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(ENA,OUTPUT);

  pinMode(2,INPUT);
  pinMode(3,INPUT);

  pinMode(20,INPUT);
  pinMode(21,INPUT);

  // interrupt for encoder 1
  attachInterrupt(0,count_A,CHANGE);//Trigger the interrupt when the votage changes
    //add interrupt for B when using quadruple frequency
  attachInterrupt(1,count_B,CHANGE);

  // interrupt for encoder 2
  attachInterrupt(2,count_A1,CHANGE);
 
  attachInterrupt(3,count_B1,CHANGE);

  pid.SetMode(AUTOMATIC);
  pid1.SetMode(AUTOMATIC);

  Serial.begin(9600);

}

void loop()
{    
   delay(100);
   update_vel();
   update_vel1();

   get_current_pos();
   get_current_pos1();


  //bluetooth communication
  if(Serial.available() > 0)  
  {
    ble_val = Serial.read();  
//    Serial.println(ble_val);

    // move forward
    if(ble_val=='a'){ 
      if(My_state=="stop" || My_dir==0){
        My_state="forward";
        My_dir=0;
        speed_follow="YES";
        //positive and negative electrodes of motor
        L=LOW;
        H=HIGH;
        L1=LOW;
        H1=HIGH;
        target=70;// set target velocity for motor 1
        target1=78;// set target velocity for motor 2
        Serial.println(My_state);}
    }

    // stop
    if(ble_val=='d'){
      My_state="stop";
      pwm_diff=0;
      L=LOW;
      H=LOW;
      L1=LOW;
      H1=LOW;
      target=0;
      target1=0;
      Serial.println(My_state);}

    // move left
    if(ble_val=='e'){
      if(My_state=="stop" || My_dir==0){
        My_state="left";
        My_dir=0;
        speed_follow="NO";
        pwm_diff=0;
        L=LOW;
        H=HIGH;
        L1=LOW;
        H1=HIGH;
        target=70;
        target1=50;
        Serial.println(My_state);}
    }

    // move right
    if(ble_val=='f'){
      if(My_state=="stop" || My_dir==0 ){
        My_state="right";
        My_dir=0;
        speed_follow="NO";
        pwm_diff=0;
        L=LOW;
        H=HIGH;
        L1=LOW;
        H1=HIGH;
        target=50;
        target1=70;
        Serial.println(My_state);}  
    } 

    // move backward
    if(ble_val=='b'){
      if(My_state=="stop"){
        My_state="backward";
        My_dir=1;
        speed_follow="YES";
        L=HIGH;
        H=LOW;
        L1=HIGH;
        H1=LOW;
        target=50;
        target1=50;
        Serial.println(My_state);}
      }

    //self rotating
    if(ble_val=='c'){
      if(My_state=="stop"){
        My_state="rotating";
        My_dir=2;
        speed_follow="YES";
        L=LOW;
        H=HIGH;
        L1=HIGH;
        H1=LOW;
        target=50;
        target1=50;
        Serial.println(My_state);}
      }
      
  }
   
   
      }
   
