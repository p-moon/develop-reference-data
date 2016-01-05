#include "s5pv210.h"
#include "int.h"

enum {
	IDLE,
	START,
	REQUEST,
	WAITING,
	RESPONSE,
	STOPING,
} one_wire_status = IDLE;

static volatile unsigned int io_bit_count;
static volatile unsigned int io_data;
static volatile unsigned char one_wire_request;

#define SAMPLE_BPS 9600
#define SLOW_LOOP_FEQ 25
#define FAST_LOOP_FEQ 60

// 三类要求:触摸、型号、背光
#define REQ_TS   0x40U		// 触摸要求
#define REQ_INFO 0x60U		// 型号要求


static int ts_ready;
static unsigned ts_status;
static int resumed = 0;

extern int gLCD_Type;

static void set_pin_as_input(void)
{
	unsigned tmp;
	tmp = GPH1CON;
	tmp &= ~(0XF << 8);
	GPH1CON = tmp;
}

static void set_pin_as_output(void)
{
	unsigned tmp;
	tmp = GPH1CON;
	tmp = (tmp & ~(0XF << 8)) | (1 << 8);
	GPH1CON = tmp;
}

static void set_pin_value(int v)
{
	unsigned tmp;
	tmp = GPH1DAT;
	if (v) {
		tmp |= (1 << 2);
	} else {
		tmp &= ~(1 << 2);
	}
	GPH1DAT = tmp;
}

static int get_pin_value(void)
{
	int v;
	unsigned long tmp;
	tmp = GPH1DAT;
	v = !!(tmp & (1 << 2));
	return v;
}

static int set_pin_up(void)
{	
	unsigned tmp;
	tmp = GPH1PUD;
	tmp = (tmp & ~(0x3 << 2*2)) | (0x2 << 2*2);
	GPH1PUD = tmp;
}

extern int gRow,gCol;
#define RGB(r,g,b)   				(unsigned int)( (r << 16) + (g << 8) + b )

////////////////////////////////////////////触摸///////////////////////////////////////////////
static  void notify_ts_data(unsigned x, unsigned y, unsigned down)
{
	//Uart_Printf("x = %d,y = %d down = %d\n",x,y,down);

	if(down)
	{
		Lcd_Printf(gRow/10*3+25*4, 0, RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"Touch screen:         ",x,y);
		Lcd_Printf(gRow/10*3+25*4, 0, RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"Touch screen:%d %d\n",x,y);
		down = 0;
	}
	if (!down && !(ts_status &(1U << 31))) {
		// up repeat, give it up
		return;
	}

	
	ts_status = ((x << 16) | (y)) | (down << 31);
	ts_ready = 1;

}

int ts_read()
{
	unsigned long err;

	// 当没有数据时，则等待
	while(!ts_ready)
		;

	return ts_status;
}



////////////////////////////////////////////背光///////////////////////////////////////////////
static int bl_ready;
static unsigned char backlight_req = 0;
static unsigned char backlight_init_success;

static  void notify_bl_data(unsigned char a, unsigned char b, unsigned char c)
{
	bl_ready = 1;
	backlight_init_success = 1;
}

static void bl_write(int brightness)
{


	if (brightness> 127) {
		brightness= 127;
	}

	bl_ready = 0;
	backlight_req = brightness + 0x80U;
}


/////////////////////////////////////////////型号///////////////////////////////////////////////
static unsigned lcd_type, firmware_ver;
static  void notify_info_data(unsigned char _lcd_type,
		unsigned char ver_year, unsigned char week)
{
	if (_lcd_type != 0xFF) {
		lcd_type = _lcd_type;
		gLCD_Type = lcd_type;
		firmware_ver = ver_year * 100 + week;
	}
	Uart_Printf("LCD type:%x\n",_lcd_type);
}

// CRC
//
static const unsigned char crc8_tab[] = {
0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15,
0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5,
0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,
0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2,
0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32,
0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C,
0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,
0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C,
0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B,
0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB,
0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB,
0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3,
};

#define crc8_init(crc) ((crc) = 0XACU)
#define crc8(crc, v) ( (crc) = crc8_tab[(crc) ^(v)])

// once a session complete
static unsigned total_received, total_error;
static unsigned last_req, last_res;
static int cnt = 0;

static void one_wire_session_complete(unsigned char req, unsigned int res)
{
	unsigned char crc;
	const unsigned char *p = (const unsigned char*)&res;
	total_received ++;


	last_res = res;


	crc8_init(crc);
	crc8(crc, p[3]);
	crc8(crc, p[2]);
	crc8(crc, p[1]);

	if (crc != p[0]) {
		// 调试时可以打开，不会影响时序
		//Uart_Printf("CRC dismatch\n");
		if (total_received > 100) {
			total_error++;
		}
		return;
	}

	//Uart_Printf("CRC match\n");
	switch(req) {
		// 触摸要求
		case REQ_TS:
			{
				unsigned short x,y;
				unsigned pressed;
				x =  ((p[3] >>   4U) << 8U) + p[2];
				y =  ((p[3] &  0xFU) << 8U) + p[1];
				pressed = (x != 0xFFFU) && (y != 0xFFFU);

				// 上报触摸数据
				// Uart_Printf("notify_ts_data\n");
				notify_ts_data(x, y, pressed);
			break;
			}
		// 型号要求
		case REQ_INFO:
			{
				// 上报信息数据
				// Uart_Printf("notify_info_data\n");
				notify_info_data(p[3], p[2], p[1]);
				break;
			}
		// 背光要求	
		default:
			{
				// 上报背光数据
				// Uart_Printf("notify_bl_data\n");
				notify_bl_data(p[3], p[2], p[1]);
				break;
			}
	}
}

// one-wire protocol core
static unsigned long TCNT_FOR_SAMPLE_BIT;
static unsigned long TCNT_FOR_FAST_LOOP;
static unsigned long TCNT_FOR_SLOW_LOOP;

// 初始化硬件定时器timer3
static void init_timer_for_1wire(void)
{
	unsigned prescale1_value;

	// get prescaler
	prescale1_value = 1;

	// calc the TCNT_FOR_SAMPLE_BIT, that is one of the goal
	TCNT_FOR_SAMPLE_BIT = 66700000 / (prescale1_value + 1) / SAMPLE_BPS - 1;
	TCNT_FOR_FAST_LOOP  = 66700000 / (prescale1_value + 1) / FAST_LOOP_FEQ - 1;
	TCNT_FOR_SLOW_LOOP  = 66700000 / (prescale1_value + 1) / SLOW_LOOP_FEQ - 1;

	// select timer 3, the 2rd goal
	TCFG1 &= ~(15<<12); 

	// Uart_Printf("TCNT_FOR_SAMPLE_BIT = %d\n",TCNT_FOR_SAMPLE_BIT);
}

static  void stop_timer_for_1wire(void)
{
	TCON &= ~(1<<16);
}

static void timer_for_1wire_interrupt()
{
	// 清中断
    TINT_CSTAT |= 0x100;
	// 清VIC
	intc_clearvectaddr();
 	io_bit_count--;
 	
	switch(one_wire_status) 
	{
	case START:
		if (io_bit_count == 0) 
		{
			io_bit_count = 16;
			one_wire_status = REQUEST;
		}
		break;

	case REQUEST:
		// 16次，每次发1bit,把要求发出去(606a)
		set_pin_value(io_data & (1U << 31));
		io_data <<= 1;
		if (io_bit_count == 0) {
			io_bit_count = 2;
			one_wire_status = WAITING;
		}
		break;
		
	case WAITING:
		// 2次
		if (io_bit_count == 0) {
			io_bit_count = 32;
			one_wire_status = RESPONSE;
		}
		if (io_bit_count == 1) {
			set_pin_as_input();
			set_pin_value(1);
		}
		break;
		
	case RESPONSE:
		// 32次，
		io_data = (io_data << 1) | get_pin_value();
		if (io_bit_count == 0) {
			io_bit_count = 2;
			one_wire_status = STOPING;
			set_pin_value(1);
			set_pin_as_output();
			// 通信完成，将要求和通信结果都传过去
			one_wire_session_complete(one_wire_request, io_data);
		}
		break;

	case STOPING:
		if (io_bit_count == 0) {
			one_wire_status = IDLE;
			stop_timer_for_1wire();
		}
		break;
		
	default:
		stop_timer_for_1wire();
	}

}


static void start_one_wire_session(unsigned char req)
{
	unsigned char crc;

	if (one_wire_status != IDLE) {
		return;
	}

	one_wire_status = START;

	set_pin_value(1);
	set_pin_as_output();

	crc8_init(crc);
	crc8(crc, req);
	io_data = (req << 8) + crc;
	io_data <<= 16;
	// Uart_Printf("start_one_wire_session crc = %x req = %x\n", crc, req);	// crc = 0x6a	req = 0x60 	io_data= 0x606a 0000

	last_req = (io_data >> 16);												// last_req = 0x606a
	one_wire_request = req;							
	io_bit_count = 1;
	set_pin_as_output();

	// 设置计数值
	TCNTB3 = TCNT_FOR_SAMPLE_BIT;
	TCMPB3 = 0;

	// 启动timer3
	TCON &= ~(0xF << 16);
	TCON |= (1<<16)|(1<<17)|(1<<19);
	TCON &= ~(1<<17);

	set_pin_value(0);

}


void One_Wire_Timer_Proc()
{
	unsigned char req;

	// 还未识别LCD类型时，第一个要求是型号要求
	if (lcd_type == 0) 
	{
		req = REQ_INFO;
	} 
	// 背光要求
	else if (!backlight_init_success) 
	{
		req = 127;
	} 
	else if (backlight_req) 
	{
		req = backlight_req;
		backlight_req = 0;
	}
	// 触摸要求
	else 
	{
		req = REQ_TS;
	}

	start_one_wire_session(req);
}

void TS_1wire_Init()
{
	set_pin_as_output();
	set_pin_value(1);
	set_pin_up();

	// 初始化硬件定时器timer3
	init_timer_for_1wire();

	// 使能timer3的中断
	pISR_TIMER3 = (unsigned long)timer_for_1wire_interrupt;
	intc_enable(NUM_TIMER3);
	TINT_CSTAT |= 0x108;

	// 开始一线处理
	One_Wire_Timer_Proc();
}

