#include <PID_v1.h> 

#define IN3 5  //定义IN3为5口
#define IN4 4  //定义IN4为4口
#define  ENB  9 //定义ENA为3口

int motor_A = 2;//中端口是0
int motor_B = 3;//中断口是1
volatile int count = 0;//如果是正转，那么每计数一次自增1，如果是反转，那么每计数一次自减1 

//double pwm=40;

int reducation = 56;//减速比，根据电机参数设置，比如 15 | 30 | 60
int pulse = 11; //编码器旋转一圈产生的脉冲数该值需要参考商家电机参数
int per_round = pulse * reducation * 4;//车轮旋转一圈产生的脉冲数 
long start_time = millis();//一个计算周期的开始时刻，初始值为 millis();
long interval_time = 50;//一个计算周期 50ms
double current_vel;

void count_A(){
  //2倍频计数实现
  //手动旋转电机一圈，输出结果为 一圈脉冲数 * 减速比 * 2
  if(digitalRead(motor_A) == HIGH){

    if(digitalRead(motor_B) == HIGH){//A 高 B 高
      count++;  
    } else {//A 高 B 低
      count--;  
    }


  } else {

    if(digitalRead(motor_B) == LOW){//A 低 B 低
      count++;  
    } else {//A 低 B 高
      count--;  
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
    } else {//B 高 A 高
      count--;
    }


  } else {

    if(digitalRead(motor_A) == HIGH){//B 低 A 高
      count++;
    } else {//B 低 A 低
      count--;
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

    Serial.println(current_vel);
    current_vel=-1*current_vel;// 控制顺时针旋转

  }
}

//-------------------------------------PID-------------------------------------------
//创建 PID 对象
//1.当前转速 2.计算输出的pwm 3.目标转速 4.kp 5.ki 6.kd 7.当输入与目标值出现偏差时，向哪个方向控制
double pwm;//电机驱动的PWM值
double target = 80;
double kp=0.5, ki=1.5, kd=0.075;
PID pid(&current_vel,&pwm,&target,kp,ki,kd,DIRECT);

//速度更新函数
void update_vel(){
  //获取当前速度
  get_current_vel();
  pid.Compute();//计算需要输出的PWM
  digitalWrite(IN3,HIGH);  //控制电机反转
  digitalWrite(IN4,LOW);
  analogWrite(ENB,pwm); //控制电机转速，迳我自己测试超过150电机才开始转

//  Serial.println(pwm);

}

void setup()
{  
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENB,OUTPUT);

  pinMode(2,INPUT);
  pinMode(3,INPUT);

  attachInterrupt(0,count_A,CHANGE);//当电平发生改变时触发中断函数
    //四倍频统计需要为B相也添加中断
  attachInterrupt(1,count_B,CHANGE);

  pid.SetMode(AUTOMATIC);

  Serial.begin(57600);

}
void loop()
{    
   delay(10);
   update_vel();
   
      }
   
