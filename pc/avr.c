char DDRB,MCUCR,ADCSRA,ADMUX,ADCSRA,PC1,ADSC,ADC,UDR,UDRE,UCSRB,TXEN,RXEN,RXCIE,UBRRL,UBRRH,UCSRA;

void sei(){};
void cli(){};

void eepromReadBlock(void* a,int b,int c){};
void eepromWriteBlock(int a,void* b,int c){};