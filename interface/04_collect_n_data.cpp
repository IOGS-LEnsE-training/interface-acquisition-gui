/**
 * FILENAME :        main_Serial.cpp          
 *
 * DESCRIPTION :
 *       Test RS232 data acquisition device / Python
 *
 * NOTES :
 *       Developped by Villou / LEnsE
 **
 * AUTHOR :    Julien VILLEMEJANE        START DATE :    25/oct/2023
 *
 *       LEnsE / Institut d'Optique Graduate School
 */
#include <cstdint>
#define     DEBUG       false

#include "mbed.h"
#include <string.h>

#define     SAMPLES_MAX     10000

/* Inputs / Outputs definitions */
DigitalOut          led1(LED1);
BufferedSerial      usb_pc(USBTX, USBRX);

Ticker              analog_tik;
AnalogIn            my_analog_sensor(A0);

/* Global Variables */
char    data;
char    cmd;
char    old_data, new_data;
bool    data_start = false;
uint8_t data_cpt = 0;
char    data_rec[6] = {'0'};
char    data2[6] = {'a','a','0','0','0','0'};
uint16_t    meas_sensor = 0;
uint16_t    meas_cpt = 0;
uint16_t    meas_auto_cpt = 0;
bool        mode_auto = false;
bool        connected = false;
uint16_t    samp_nb = 0;        // number of samples
uint16_t    samp_freq = 1;      // sampling frequency
uint16_t    samp_ind = 0;       // sample required
uint16_t    samples[SAMPLES_MAX];          // samples

/* Others functions declarations */
/**
* @brief Interrupt sub-routine function for RX receiving from PC (serial)
*/
void    usb_pc_ISR(void);
/**
* @brief Interrupt sub-routine function for analog-to-digital converter
*/
void    analog_tik_ISR(void);

/* MAIN PROGRAM */
int main()
{
    // Initialization of Serial communication port
    usb_pc.set_baud(115200);
    usb_pc.set_format(8, SerialBase::None, 1);
    // Set Interrupt for RX data
    usb_pc.sigio(callback(usb_pc_ISR));

    while (true)
    {
        wait_us(100);
    }
}



/**
* @brief Create and send packet composed of 6 bytes
* @details Create and send packet composed of 6 bytes :
*       * byte 1 : cmd 
*       * byte 2 : cmd (repeated) 
*       * byte 3 : index (MSB) 
*       * byte 4 : index (LSB) 
*       * byte 5 : value (MSB)
*       * byte 6 : value (LSB)
* @param cmd command received from the host
* @param index index of the value returns to the host
* @param value value to return to the host
*/
void    send_packet(uint8_t cmd, uint16_t index, uint16_t value){
    data2[0] = cmd;
    data2[1] = cmd;
    data2[2] = index >> 8;
    data2[3] = index;
    data2[4] = value >> 8;
    data2[5] = value;
    usb_pc.write(&data2, 6); // send data
}

/**
* @brief Create and send debug packet composed of 6 bytes
* @details Create and send packet composed of 6 bytes :
*       * byte 1 : '-' 
*       * byte 2 : cmd
*       * byte 3 : '/'
*       * byte 4 : index 
*       * byte 5 : value
*       * byte 6 : '\n'
* @param cmd command received from the host
* @param index index of the value returns to the host
* @param value value to return to the host
*/
void    send_debug(uint8_t cmd, uint16_t index, uint16_t value){
    if(DEBUG){
        data2[0] = '-';
        data2[1] = cmd;
        data2[2] = '/';
        data2[3] = index;
        data2[4] = value;
        data2[5] = '\n';
        usb_pc.write(&data2, 6); // send data
    }
}


/**
* @brief Test if board is connected and send an acknowledgement
* @param send_ack index of the value returns to the host - default false
* @return true if board is connected
*/
bool    is_connected(bool send_ack=false){
    if(send_ack){
        if(!connected)
            send_packet('i', 0, 0); // send ack 'i' - first connection
        else
            send_packet('j', 0, 0); // send ack 'j' - already connected
    }
    return connected;
}


void    usb_pc_ISR(void){
    if(usb_pc.readable()){
        usb_pc.read(&data, 1);

        // Detection of start of frame
        new_data = data;
        send_debug('n', new_data, '_');
        send_debug('o', old_data, '_');
        if(new_data == old_data){
            data_start = true;
            data_cpt = 0;
            cmd = data;
            send_debug('S', '0'+data_cpt, '\n');
        }
        else{
            if(data_start == true){
                led1 = !led1;
                send_debug('+', '0'+data_cpt, '\n');
                data_rec[data_cpt] = data;
                data_cpt++;
                if(data_cpt > 3){
                    data_start = false;
                }
            }
        }
        old_data = new_data;

        // When 4 bytes following a starting command...
        if((data_cpt > 3) && (data_start == false)){
            data_cpt = 0;
            send_debug('C', cmd, '\n');
            switch(cmd){
                case 'i':       // Initialization
                    is_connected(true);
                    connected = true;
                    break;
                case 'a':       // Automatic acquisition
                    if(is_connected()){
                        mode_auto = true;
                        samp_nb = (data_rec[0] << 8) + data_rec[1];
                        samp_freq = (data_rec[2] << 8) + data_rec[3];
                        send_packet('a', samp_nb, samp_freq); // send ack
                        double samp_time = 1.0/samp_freq;                        
                        analog_tik.attach(&analog_tik_ISR, samp_time);
                    }
                    else{
                        send_packet('c', 0, 0); // send ack
                    }
                    break;
                case 'm':       // Manual acquisition
                    if(is_connected()){
                        meas_cpt++;
                        meas_sensor = my_analog_sensor.read_u16() >> 4;
                        send_packet('m', meas_cpt, meas_sensor);
                        mode_auto = false;
                        analog_tik.detach();
                    }
                    else{
                        send_packet('c', 0, 0); // send ack
                    }
                    break;
                case 'd':       // Data transmission
                    if(is_connected()){
                        if(mode_auto == true){
                            send_packet('d', 0, 0); // send ack
                        }
                        else{
                            samp_ind = (data_rec[0] << 8) + data_rec[1]; 
                            send_packet('d', samp_ind, samples[samp_ind-1]); // send ack
                        }
                    }
                    else{
                        send_packet('c', 0, 0); // send ack
                    }
                    break;
                default :
                    send_packet(data, 0, 0);
            }    
        }
    }
}


void    analog_tik_ISR(void){
    if(mode_auto == true){
        samples[meas_auto_cpt] = my_analog_sensor.read_u16() >> 4;
        meas_auto_cpt++;
        if(meas_auto_cpt == samp_nb){
            mode_auto = false;
            analog_tik.detach();
        }
    }
}