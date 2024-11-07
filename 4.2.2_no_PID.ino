/*
代码模块解析：
1. 四轮差速驱动控制
2. WI-FI网页控制
3. PWM速度调节
4. 基本运动控制（前进/后退/转向）
*/

#include <webpage422no_PID.h>
#include <html510.h>

//定义四个电机引脚

//左前LF
#define LF_forward 
#define LF_backward
#define LF_PWM_speed

//右前RL
#define RF_forward
#define RF_backward
#define RF_PWM_speed

//左后LB
#define LB_forward
#define LB_backward
#define LB_PWM_speed

//右后RB
#define RB_forward
#define RB_backward
#define RB_PWM_speed

//WI-FI 参数配置
const char* ssid = "Stone";
const char* password = "";
HTML510Server h(80);

//PWM 参数设置
#define FREQUENCY 10000
#define RESOLUTION 12 //0-4095

// 速度参数
const int default_duty = 90;    // 默认占空比90%

// 初始化
void setup() {
  pwm_setup();      
  motors_setup();    
  server_setup(); 
}

void loop() {
  h.serve();        
  delay(10);        
}

// PWM初始化
void pwm_setup() {

  ledcAttach(LF_PWM_speed, FREQUENCY, RESOLUTION); 
  ledcWrite(LF_PWM_speed, 0); 


  ledcAttach(RF_PWM_speed, FREQUENCY, RESOLUTION); 
  ledcWrite(RF_PWM_speed, 0); 


  ledcAttach(LB_PWM_speed, FREQUENCY, RESOLUTION); 
  ledcWrite(LB_PWM_speed, 0); 


  ledcAttach(RB_PWM_speed, FREQUENCY, RESOLUTION); 
  ledcWrite(RB_PWM_speed, 0); 

// 电机引脚初始化函数
void motors_setup() {

  pinMode(LF_forward, OUTPUT);   
  pinMode(LF_backward, OUTPUT); 

 
  pinMode(RF_forward, OUTPUT);   
  pinMode(RF_backward, OUTPUT);   

  
  pinMode(LB_forward, OUTPUT);    
  pinMode(LB_backward, OUTPUT);    


  pinMode(RB_forward, OUTPUT);    
  pinMode(RB_backward, OUTPUT);   
}

// 连接 Wi-Fi
  Serial.begin(115200);
  Serial.print("Access Point ");
  Serial.print(ssid);
  WiFi.softAP(ssid,password);
  WiFi.softAPConfig(IPAddress(192, 168, 1, 140), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));


// Web服务器初始化
void server_setup() {
  h.begin();   
  // 注册所有的URL处理函数
  h.attachHandler("/ ", handleRoot);              // 处理根路径，显示控制界面
  h.attachHandler("/forwards ", handleForwards);  // 处理前进命令
  h.attachHandler("/backwards ", handleBackwards);// 处理后退命令
  h.attachHandler("/stop ", handleStop);         // 处理停止命令
  h.attachHandler("/right ", handleRight);       // 处理右转命令
  h.attachHandler("/left ", handleLeft);         // 处理左转命令
}

// 根路径处理函数 - 发送HTML控制界面
void handleRoot() {
  h.sendhtml(body);    
}

// 前进命令处理函数
void handleForwards() {
  //所有电机为正向默认速度
  set_motor_speed(1, default_duty);  
  set_motor_speed(2, default_duty);   
  set_motor_speed(3, default_duty); 
  set_motor_speed(4, default_duty); 
}

// 后退命令处理函数
void handleBackwards() {
  //所有电机为反向默认速度
  set_motor_speed(1, -default_duty);  
  set_motor_speed(2, -default_duty);  
  set_motor_speed(3, -default_duty);  
  set_motor_speed(4, -default_duty);  
}

// 停止命令处理函数
void handleStop() {
  //所有轮子速度全部为0
  set_motor_speed(1, 0);   
  set_motor_speed(2, 0);   
  set_motor_speed(3, 0);   
  set_motor_speed(4, 0);   
}

// 右转命令处理函数
void handleRight() {
  //左两轮的速度是右两轮的2倍
  set_motor_speed(1, default_duty);  
  set_motor_speed(2, default_duty * 0.5);   
  set_motor_speed(3, default_duty);   
  set_motor_speed(4, default_duty * 0.5);  
}

// 左转命令处理函数
void handleLeft() {
  //右两轮的速度是左两轮的2倍
  set_motor_speed(1, default_duty * 0.5);   
  set_motor_speed(2, default_duty);  
  set_motor_speed(3, default_duty * 0.5);  
  set_motor_speed(4, default_duty);   
}



// 电机核心控制函数
/**
* 设置单个电机的速度和方向
* motorNumber: 电机编号 1=左前, 2=右前, 3=左后, 4=右后
* speedValue: 速度值 
* 正值(0~100): 正转
* 负值(-100~0): 反转
* 0: 停止
*/
void set_motor_speed(unsigned int motorNumber, int speedValue) {
   // 定义电机控制引脚
   uint8_t forwardPin;   // 正转控制引脚
   uint8_t backwardPin;  // 反转控制引脚 
   uint8_t pwmPin;   // PWM通道

   // 根据电机编号选择对应的控制引脚
   switch(motorNumber) {
       case 1:  // 左前电机
           forwardPin = LF_forward;
           backwardPin = LF_backward;
           pwmPin = LF_PWM_speed;
           break;
       case 2:  // 右前电机
           forwardPin = RF_forward;
           backwardPin = RF_backward;
           pwmPin = RF_PWM_speed;
           break;
       case 3:  // 左后电机
           forwardPin = LB_forward;
           backwardPin = LB_backward;
           pwmPin = LB_PWM_speed;
           break;
       case 4: // 右后电机
           forwardPin = RB_forward;
           backwardPin = RB_backward;
           pwmPin = RB_PWM_speed;
           break;
   }

   // 设置电机转动方向
   if (speedValue == 0) {
       // 停止: 两个方向控制引脚都设为LOW
       digitalWrite(forwardPin, LOW);
       digitalWrite(backwardPin, LOW);
   } 
   else if (speedValue > 0) {
       // 正转: 正转引脚HIGH，反转引脚LOW
       digitalWrite(forwardPin, HIGH);
       digitalWrite(backwardPin, LOW);
   } 
   else {
       // 反转: 正转引脚LOW，反转引脚HIGH
       digitalWrite(forwardPin, LOW);
       digitalWrite(backwardPin, HIGH);
   }

   // 设置电机速度 (PWM占空比)
   int pwmValue = map(abs(speedValue), 0, 100, 0, 4095);
   ledcWrite(pwmPin, pwmValue);
}





