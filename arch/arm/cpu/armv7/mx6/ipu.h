/***************************************************************************
*
*    ipu.h
*
*    IPU initialization, connect each module and build a link from memory to  
* display .
*
*
***************************************************************************/

#if 0
typedef struct {
	unsigned int channel;
	unsigned int xv;
	unsigned int yv;
	unsigned int xb;
	unsigned int yb;
	unsigned int nsb_b;
	unsigned int cf;
	unsigned int sx;
	unsigned int sy;
	unsigned int ns;
	unsigned int sdx;
	unsigned int sm;
	unsigned int scc;
	unsigned int sce;
	unsigned int sdy;
	unsigned int sdrx;
	unsigned int sdry;
	unsigned int bpp;
	unsigned int dec_sel;
	unsigned int dim;
	unsigned int so;
	unsigned int bndm;
	unsigned int bm;
	unsigned int rot;
	unsigned int hf;
	unsigned int vf;
	unsigned int the;
	unsigned int cap;
	unsigned int cae;
	unsigned int fw;
	unsigned int fh;
	unsigned int eba0;
	unsigned int eba1;
	unsigned int ilo;
	unsigned int npb;
	unsigned int pfs;
	unsigned int alu;
	unsigned int albm;
	unsigned int id;
	unsigned int th;
	unsigned int sl;
	unsigned int wid0;
	unsigned int wid1;
	unsigned int wid2;
	unsigned int wid3;
	unsigned int ofs0;
	unsigned int ofs1;
	unsigned int ofs2;
	unsigned int ofs3;
	unsigned int ubo;
	unsigned int vbo;
}ipu_channel_parameter_t;

#endif

void di_config_interlaced(unsigned int IPU, unsigned int DI, int div, int extern_clock,
	int clock_polarity, int hsync_polarity, int vsync_polarity, int data_polarity, int data_polarity_enable,
	int hSyncStartWidth, int hSyncWidth, int hSyncEndWidth, int hDisp,
	int vSyncStartWidth, int vSyncWidth, int vSyncEndWidth, int vDisp);
	
void di_config_none_interlaced(unsigned int IPU, unsigned int DI, int div, int extern_clock,
	int clock_polarity, int hsync_polarity, int vsync_polarity, int data_polarity, int data_polarity_enable,
	int hSyncStartWidth, int hSyncWidth, int hSyncEndWidth, int hDisp,
	int vSyncStartWidth, int vSyncWidth, int vSyncEndWidth, int vDisp);

void dc_config_interlaced(unsigned int IPU, unsigned int DI, unsigned int width, int mapping);

void dc_config_none_interlaced(unsigned int IPU, unsigned int DI, unsigned int width, int mapping);

void enable_display(unsigned int IPU, unsigned int DI);

void dp_config(unsigned int IPU);

void dmfc_config(unsigned int IPU);

void dp_enable(unsigned int IPU);

void idmac_config(unsigned int IPU, unsigned int bpp, unsigned int width, unsigned int height, unsigned eba);

void ipu_disable_all(void);
void ipu_set_lvds_clock_split(unsigned int IPU, unsigned int DI, unsigned int pixel_clock);
void ipu_set_lvds_clock(unsigned int IPU, unsigned int DI, unsigned int pixel_clock);
void ipu_set_hdmi_clock(unsigned int IPU, unsigned int DI, unsigned int pixel_clock);
void ipu_set_lcd_clock(unsigned int IPU, unsigned int DI, unsigned int pixel_clock);
void ipu_startup(unsigned int IPU, unsigned int DI);
void ipu_enable_lvds_clock(void);
void ipu_enable_hmdi_clock(void);


void setup_hdmi_iomux(unsigned int IPU, unsigned int DI);

void setup_lvds_aux(void);
void setup_lvds_iomux_port_0(unsigned int IPU, unsigned int DI);
void setup_lvds_iomux_port_1(unsigned int IPU, unsigned int DI);
void setup_lvds_port_0(unsigned int DI);
void setup_lvds_port_1(unsigned int DI);