#include <PID_v1.h> 

#define IN3 5  //定义IN3为5口
#define IN4 4  //定义IN4为4口
#define  ENB  9 //定义ENA为9口

#define IN1 6  //定义IN3为6口
#define IN2 7  //定义IN4为7口
#define  ENA  10 //定义ENA为10口

int t=0;

int motor_A = 2;//中端口是0
int motor_B = 3;//中断口是1
volatile int count = 0;//如果是正转，那么每计数一次自增1，如果是反转，那么每计数一次自减1 
volatile int pos = 0;

int motor_A1 = 21;//中端口是0
int motor_B1 = 20;//中断口是1
volatile int count1 = 0;//如果是正转，那么每计数一次自增1，如果是反转，那么每计数一次自减1 
volatile int pos1 = 0;


double pwm_diff=0;

//double pwm=40;

int reducation = 56;//减速比，根据电机参数设置，比如 15 | 30 | 60
int pulse = 11; //编码器旋转一圈产生的脉冲数该值需要参考商家电机参数
int per_round = pulse * reducation * 4;//车轮旋转一圈产生的脉冲数 
long start_time = millis();//一个计算周期的开始时刻，初始值为 millis();
long interval_time = 50;//一个计算周期 50ms
double current_vel;
double angle;

int reducation1 = 56;//减速比，根据电机参数设置，比如 15 | 30 | 60
int pulse1 = 11; //编码器旋转一圈产生的脉冲数该值需要参考商家电机参数
int per_round1 = pulse1 * reducation1 * 4;//车轮旋转一圈产生的脉冲数 
long start_time1 = millis();//一个计算周期的开始时刻，初始值为 millis();
long interval_time1 = 50;//一个计算周期 50ms
double current_vel1;
double angle1;

void count_A(){
  //2倍频计数实现
  //手动旋转电机一圈，输出结果为 一圈脉冲数 * 减速比 * 2
  if(digitalRead(motor_A) == HIGH){

    if(digitalRead(motor_B) == HIGH){//A 高 B 高
      count++;  
      pos++;
    } else {//A 高 B 低
      count--;  
      pos--;
    }


  } else {

    if(digitalRead(motor_B) == LOW){//A 低 B 低
      count++;  
      pos++;
    } else {//A 低 B 高
      count--;  
      pos--;
    }  

  }

}

//与A实现类似
//4倍频计数实现
//手动旋转电机一圈，输出结果为 一圈脉冲数 * 减速比 * 4
void count_B(){
  if(digitalRead(motor_B) == HIGH){

    if(digitalRead(motor_A) == LOW){//B 高 A 低
      count++;
      pos++;
    } else {//B 高 A 高
      count--;
      pos--;
    }


  } else {

    if(digitalRead(motor_A) == HIGH){//B 低 A 高
      count++;
      pos++;
    } else {//B 低 A 低
      count--;
      pos--;
    }

  }

}

//获取当前转速的函数
void get_current_vel(){
  long right_now = millis();  
  long past_time = right_now - start_time;//计算逝去的时间
  if(past_time >= interval_time){//如果逝去时间大于等于一个计算周期
    //1.禁止中断
    noInterrupts();
    //2.计算转速 转速单位可以是秒，也可以是分钟... 自定义即可
    current_vel = (double)count / per_round / past_time * 1000 * 60;
    //3.重置计数器
    count = 0;
    //4.重置开始时间
    start_time = right_now;
    //5.重启中断
    interrupts();

    Serial.print(-current_vel);
    Serial.print(",");

    current_vel=-1*current_vel; //顺时针旋转

  }
}


void count_A1(){
  //2倍频计数实现
  //手动旋转电机一圈，输出结果为 一圈脉冲数 * 减速比 * 2
  if(digitalRead(motor_A1) == HIGH){

    if(digitalRead(motor_B1) == HIGH){//A 高 B 高
      count1++;
      pos1++;  
    } else {//A 高 B 低
      count1--;  
      pos1--;
    }


  } else {

    if(digitalRead(motor_B1) == LOW){//A 低 B 低
      count1++; 
      pos1++; 
    } else {//A 低 B 高
      count1--;  
      pos1--;
    }  

  }

}

//与A实现类似
//4倍频计数实现
//手动旋转电机一圈，输出结果为 一圈脉冲数 * 减速比 * 4
void count_B1(){
  if(digitalRead(motor_B1) == HIGH){

    if(digitalRead(motor_A1) == LOW){//B 高 A 低
      count1++;
      pos1++;
    } else {//B 高 A 高
      count1--;
      pos1--;
    }


  } else {

    if(digitalRead(motor_A1) == HIGH){//B 低 A 高
      count1++;
      pos1++;
    } else {//B 低 A 低
      count1--;
      pos1--;
    }

  }

}

//获取当前转速的函数
void get_current_vel1(){
  long right_now = millis();  
  long past_time = right_now - start_time1;//计算逝去的时间
  if(past_time >= interval_time1){//如果逝去时间大于等于一个计算周期
    //1.禁止中断
    noInterrupts();
    //2.计算转速 转速单位可以是秒，也可以是分钟... 自定义即可
    current_vel1 = (double)count1 / per_round1 / past_time * 1000 * 60;
    //3.重置计数器
    count1 = 0;
    //4.重置开始时间
    start_time1 = right_now;
    //5.重启中断
    interrupts();

    Serial.println(current_vel1);

//    current_vel1=-1*current_vel1;//逆时针旋转

  }
}

//获取当前位置的函数
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

//获取当前位置的函数
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
//创建 PID 对象
//1.当前转速 2.计算输出的pwm 3.目标转速 4.kp 5.ki 6.kd 7.当输入与目标值出现偏差时，向哪个方向控制
double pwm;//电机驱动的PWM值
double target = 20;
double kp=0.5, ki=1.5, kd=0.075;
uint8_t L=LOW;
uint8_t H=HIGH;
PID pid(&current_vel,&pwm,&target,kp,ki,kd,DIRECT);

//速度更新函数
void update_vel(){
  //获取当前速度
  get_current_vel();
  pid.Compute();//计算需要输出的PWM
  digitalWrite(IN3,H);  //控制电机反转
  digitalWrite(IN4,L);
  analogWrite(ENB,pwm); //控制电机转速，迳我自己测试超过150电机才开始转

//  Serial.println(pwm);

}


//-------------------------------------PID-------------------------------------------
//创建第二个 PID 对象
//1.当前转速 2.计算输出的pwm 3.目标转速 4.kp 5.ki 6.kd 7.当输入与目标值出现偏差时，向哪个方向控制
double pwm1;//电机驱动的PWM值
double target1 = 20;
double kp1=0.5, ki1=1.5, kd1=0.075;
PID pid1(&current_vel1,&pwm1,&target1,kp1,ki1,kd1,DIRECT);

//速度更新函数
void update_vel1(){
  //获取当前速度
  get_current_vel1();
  pid1.Compute();//计算需要输出的PWM
  digitalWrite(IN1,LOW);  //控制电机反转
  digitalWrite(IN2,HIGH);
  analogWrite(ENA,pwm1+pwm_diff); //控制电机转速，迳我自己测试超过150电机才开始转

//两个电机速度做补偿（速度不同时需要注释）注意：加上速度跟随会显著增加电机的震荡
//  if(current_vel1<current_vel){pwm_diff=pwm_diff+0.05;}
//  else if(current_vel1==current_vel){pwm_diff=0;} 
//  else if(current_vel1>current_vel){pwm_diff=pwm_diff-0.05;}

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

  attachInterrupt(0,count_A,CHANGE);//当电平发生改变时触发中断函数
    //四倍频统计需要为B相也添加中断
  attachInterrupt(1,count_B,CHANGE);

  attachInterrupt(2,count_A1,CHANGE);//当电平发生改变时触发中断函数
    //四倍频统计需要为B相也添加中断
  attachInterrupt(3,count_B1,CHANGE);

  pid.SetMode(AUTOMATIC);
  pid1.SetMode(AUTOMATIC);

  Serial.begin(57600);

}
void loop()
{    
   delay(100);
   update_vel();
   update_vel1();

   get_current_pos();
   get_current_pos1();


//   if(angle1-angle<20 && angle1-angle>-20){Serial.println(angle1-angle);}

//  Serial.println(angle1+angle);//两个电机旋转方向相反时差值用+号
      }
   
