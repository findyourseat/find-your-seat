/**
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include "mbed.h"

#include "lorawan/LoRaWANInterface.h"
#include "lorawan/system/lorawan_data_structures.h"
#include "events/EventQueue.h"

// Application helpers
#include "trace_helper.h"
#include "lora_radio_helper.h"

// Force sensor helper
#define         SENSOR1_PIN     A0			// Pin of the first force sensor
#define         SENSOR2_PIN     A2			// Pin of the second force sensor
#define         TEMP            PA_4		// Pin of the thermistor (temperature sensor) 
AnalogIn        sensor1(SENSOR1_PIN);
AnalogIn        sensor2(SENSOR2_PIN);
AnalogIn        temp(TEMP);
char             getSensorValue(AnalogIn force_sensor);
int            getTemp();
// End sensor helper

using namespace events;

// Max payload size can be LORAMAC_PHY_MAXPAYLOAD.
// This example only communicates with much shorter messages (<30 bytes).
// If longer messages are used, these buffers must be changed accordingly.
uint8_t tx_buffer[30];
uint8_t rx_buffer[30];

/*
 * Sets up an application dependent transmission timer in ms. Used only when Duty Cycling is off for testing
 */
#define TX_TIMER                        2500

/**
 * Maximum number of events for the event queue.
 * 10 is the safe number for the stack events, however, if application
 * also uses the queue for whatever purposes, this number should be increased.
 */
#define MAX_NUMBER_OF_EVENTS            10

/**
 * Maximum number of retries for CONFIRMED messages before giving up
 */
#define CONFIRMED_MSG_RETRY_COUNTER     3


/**
* This event queue is the global event queue for both the
* application and stack. To conserve memory, the stack is designed to run
* in the same thread as the application and the application is responsible for
* providing an event queue to the stack that will be used for ISR deferment as
* well as application information event queuing.
*/
static EventQueue ev_queue(MAX_NUMBER_OF_EVENTS *EVENTS_EVENT_SIZE);

/**
 * Event handler.
 *
 * This will be passed to the LoRaWAN stack to queue events for the
 * application which in turn drive the application.
 */
static void lora_event_handler(lorawan_event_t event);

/**
 * Constructing Mbed LoRaWANInterface and passing it the radio object from lora_radio_helper.
 */
static LoRaWANInterface lorawan(radio);

/**
 * Application specific callbacks
 */
static lorawan_app_callbacks_t callbacks;

/**
 * Entry point for application
 */
int main(void)
{
    // setup tracing
    setup_trace();

    // stores the status of a call to LoRaWAN protocol
    lorawan_status_t retcode;

    // Initialize LoRaWAN stack
    if (lorawan.initialize(&ev_queue) != LORAWAN_STATUS_OK) {
        printf("\r\n LoRa initialization failed! \r\n");
        return -1;
    }

    printf("\r\n Mbed LoRaWANStack initialized \r\n");

    // prepare application callbacks
    callbacks.events = mbed::callback(lora_event_handler);
    lorawan.add_app_callbacks(&callbacks);

    // Set number of retries in case of CONFIRMED messages
    if (lorawan.set_confirmed_msg_retries(CONFIRMED_MSG_RETRY_COUNTER)
            != LORAWAN_STATUS_OK) {
        printf("\r\n set_confirmed_msg_retries failed! \r\n\r\n");
        return -1;
    }

    printf("\r\n CONFIRMED message retries : %d \r\n",
           CONFIRMED_MSG_RETRY_COUNTER);

    // Enable adaptive data rate
    if (lorawan.enable_adaptive_datarate() != LORAWAN_STATUS_OK) {
        printf("\r\n enable_adaptive_datarate failed! \r\n");
        return -1;
    }

    printf("\r\n Adaptive data  rate (ADR) - Enabled \r\n");

    retcode = lorawan.connect();

    if (retcode == LORAWAN_STATUS_OK ||
            retcode == LORAWAN_STATUS_CONNECT_IN_PROGRESS) {
    } else {
        printf("\r\n Connection error, code = %d \r\n", retcode);
        return -1;
    }

    printf("\r\n Connection - In Progress ...\r\n");

    // make your event queue dispatching events forever
    ev_queue.dispatch_forever();

    return 0;
}

/**
 * Retrieve value from force sensors
 */
char getSensorValue(AnalogIn force_sensor){
    int value = force_sensor.read();
    return value >= 0.6 ? char(0):char(1);
}

/**
 * Retrieve value from temp sensor
 */
int getTemp(){
    float thr_val=0, thr_res=0, temperature=0;
    thr_val = (temp.read()*4095);
    thr_val = ((thr_val*3.3)/4095);
    thr_res = log(((3.3*(10/thr_val))-10)*1000);
    temperature = ((1/(0.001129148+(0.000234125*thr_res)+(0.0000000876741*thr_res*thr_res*thr_res)))-273.15);
    return temperature > 0 ? int(temperature):25;
}

int i = 0;
/**
 * Sends a message to the Network Server
 */
static void send_message() {
    uint16_t packet_len;
    int16_t retcode;
    char force_sensor_value1;
    char force_sensor_value2;
    int temp_sensor_value;
    
    switch (i % 3 ) {
        // force sensor 1
        case 0: force_sensor_value1 = getSensorValue(sensor1);

                printf("Seat 1 free = %d\n", int(force_sensor_value1));

                packet_len = sprintf((char *) tx_buffer, "%c", force_sensor_value1);

                retcode = lorawan.send(1, tx_buffer, packet_len, MSG_UNCONFIRMED_FLAG);

                if (retcode < 0) {
                    retcode == LORAWAN_STATUS_WOULD_BLOCK ? printf("send - WOULD BLOCK\r\n")
                    : printf("\r\n send() - Error code %d \r\n", retcode);

                    if (retcode == LORAWAN_STATUS_WOULD_BLOCK) {
                        //retry in 3 seconds
                        if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                            ev_queue.call_in(3000, send_message);
                        }
                    }
                    return;
                }
                
                printf("\r\n %d bytes scheduled for transmission \r\n", retcode);
                memset(tx_buffer, 0, sizeof(tx_buffer));
                i++;
                break;
        // Force sensor 2
        case 1: force_sensor_value2 = getSensorValue(sensor2);

                printf("Seat 2 free = %d\n", int(force_sensor_value2));

                packet_len = sprintf((char *) tx_buffer, "%c", force_sensor_value2);

                retcode = lorawan.send(2, tx_buffer, packet_len, MSG_UNCONFIRMED_FLAG);

                if (retcode < 0) {
                    retcode == LORAWAN_STATUS_WOULD_BLOCK ? printf("send - WOULD BLOCK\r\n")
                    : printf("\r\n send() - Error code %d \r\n", retcode);

                    if (retcode == LORAWAN_STATUS_WOULD_BLOCK) {
                        //retry in 3 seconds
                        if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                            ev_queue.call_in(3000, send_message);
                        }
                    }
                    return;
                }
                
                printf("\r\n %d bytes scheduled for transmission \r\n", retcode);
                memset(tx_buffer, 0, sizeof(tx_buffer));
                i++;
                break;

        // Temperature sensor
        case 2: temp_sensor_value = getTemp();

                printf("Temperature = %d\n", temp_sensor_value);

                packet_len = sprintf((char *) tx_buffer, "%c", char(temp_sensor_value) );

                retcode = lorawan.send(3, tx_buffer, packet_len, MSG_UNCONFIRMED_FLAG);

                if (retcode < 0) {
                    retcode == LORAWAN_STATUS_WOULD_BLOCK ? printf("send - WOULD BLOCK\r\n")
                    : printf("\r\n send() - Error code %d \r\n", retcode);

                    if (retcode == LORAWAN_STATUS_WOULD_BLOCK) {
                        //retry in 3 seconds
                        if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                            ev_queue.call_in(3000, send_message);
                        }
                    }
                    return;
                } 
                printf("\r\n %d bytes scheduled for transmission \r\n", retcode);
                memset(tx_buffer, 0, sizeof(tx_buffer));
                i++;
                break;
    }
}

/**
 * Receive a message from the Network Server
 */
static void receive_message()
{
    uint8_t port;
    int flags;
    int16_t retcode = lorawan.receive(rx_buffer, sizeof(rx_buffer), port, flags);

    if (retcode < 0) {
        printf("\r\n receive() - Error code %d \r\n", retcode);
        return;
    }

    printf(" RX Data on port %u (%d bytes): ", port, retcode);
    for (uint8_t i = 0; i < retcode; i++) {
        printf("%02x ", rx_buffer[i]);
    }
    printf("\r\n");
    
    memset(rx_buffer, 0, sizeof(rx_buffer));
}

/**
 * Event handler
 */
static void lora_event_handler(lorawan_event_t event)
{
    switch (event) {
        case CONNECTED:
            printf("\r\n Connection - Successful \r\n");
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                send_message();
            } else {
                ev_queue.call_every(TX_TIMER, send_message);
            }

            break;
        case DISCONNECTED:
            ev_queue.break_dispatch();
            printf("\r\n Disconnected Successfully \r\n");
            break;
        case TX_DONE:
            printf("\r\n Message Sent to Network Server \r\n");
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                send_message();
            }
            break;
        case TX_TIMEOUT:
        case TX_ERROR:
        case TX_CRYPTO_ERROR:
        case TX_SCHEDULING_ERROR:
            printf("\r\n Transmission Error - EventCode = %d \r\n", event);
            // try again
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                send_message();
            }
            break;
        case RX_DONE:
            printf("\r\n Received message from Network Server \r\n");
            receive_message();
            break;
        case RX_TIMEOUT:
        case RX_ERROR:
            printf("\r\n Error in reception - Code = %d \r\n", event);
            break;
        case JOIN_FAILURE:
            printf("\r\n OTAA Failed - Check Keys \r\n");
            break;
        case UPLINK_REQUIRED:
            printf("\r\n Uplink required by NS \r\n");
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                send_message();
            }
            break;
        default:
            MBED_ASSERT("Unknown Event");
    }
}

// EOF
