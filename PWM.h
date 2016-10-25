

/**** TIM1_CH1(PA8) as PWM. f=50Hz ***/
void PWM_Init(void);

/**** basit delay ile git gel ****/
void ServoSur(int hiz);

/**** gonderilen aci degerini anlamli hale cevirip o aci konumuna gececek. ****/
void ServoKonum(int Pulse);
