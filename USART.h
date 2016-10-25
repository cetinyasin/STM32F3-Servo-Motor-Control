
// USART1 Init
void USART1_Init(void);


/**** send string ****/
void USART1_Puts(volatile char *s);

/***********************************/
// 				int -->>>  str
/***********************************/
char* intToStr(int32_t data);

/* main'den ulasip servoya basmak icin */
int getRxBuff(void);

/* 									*/
void data_process(void);


int getKontrolInfo(void);

