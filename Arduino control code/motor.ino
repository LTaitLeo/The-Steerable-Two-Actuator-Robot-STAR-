#define IN3 5  //定义IN3为5口
#define IN4 4  //定义IN4为4口
#define IN1 8  //定义IN3为5口
#define IN2 7  //定义IN4为4口
#define  ENA  3 //定义ENA为3口
#define  ENB  9 //定义ENA为3口

void setup()
{  
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(ENA,OUTPUT);

}
void loop()
{   


   digitalWrite(IN1,LOW);  //控制电机反转
   digitalWrite(IN2,HIGH);
   analogWrite(ENB,255);
   
   digitalWrite(IN3,LOW);  //控制电机反转
   digitalWrite(IN4,HIGH);
   analogWrite(ENA,255);   //控制电机转速，迳我自己测试超过150电机才开始转
   delay(5000);//持续时间

   digitalWrite(IN1,LOW);  //控制电机停止
   digitalWrite(IN2,LOW);
   digitalWrite(IN3,LOW);  //控制电机停止
   digitalWrite(IN4,LOW);
   delay(5000);//持续时间
      }
   
