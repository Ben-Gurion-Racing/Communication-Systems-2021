/**
 ******************************************************************************
  * File Name          : LWIP.c
  * Description        : This file provides initialization code for LWIP
  *                      middleWare.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#if defined ( __CC_ARM )  /* MDK ARM Compiler */
#include "lwip/sio.h"
#endif /* MDK ARM Compiler */

/* USER CODE BEGIN 0 */
#include "lwip/udp.h"

/* USER CODE END 0 */
/* Private function prototypes -----------------------------------------------*/
/* ETH Variables initialization ----------------------------------------------*/
void _Error_Handler(char * file, int line);

/* USER CODE BEGIN 1 */
static struct udp_pcb* upcb;
static struct udp_pcb* upcb2;

uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];

struct pbuf *pp;

//==================== FLAG =========================

extern volatile unsigned char motor_RIGHT ;
extern volatile unsigned char motor_LEFT ;



/* USER CODE END 1 */

/* Variables Initialization */
struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];

/* USER CODE BEGIN 2 */

void udp_receive_callback_L(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port){

static char buffer[20];

printf("\r reacive \n");
      if (p != NULL)
      {
    	  if(p->len > 2)
    	  {
    	      pbuf_copy_partial(p, buffer, p->tot_len, 0);
    	      if(buffer[0] == 'M' && buffer[1] == 'O' && buffer[2] == '\r')
    	          	  {
    	          		  //buff[0] = *(p->payload+3);
    	    	  	  	  motor_LEFT = buffer[3] - 48;
    	    	  	  	  printf("\r mototr left = %d",motor_LEFT);
    	          	  }
    	  }

        /* copy data to pbuf */
        /* send udp data */
        /* free pbuf */
        pbuf_free(p);
      }
}



void udp_receive_callback_R(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port){

static char buffer[20];

printf("\r reacive \n");
      if (p != NULL)
      {
    	  if(p->len > 2)
    	  {
    	      pbuf_copy_partial(p, buffer, p->tot_len, 0);
    	      if(buffer[0] == 'M' && buffer[1] == 'O' && buffer[2] == '\r')
    	          	  {
    	          		  //buff[0] = *(p->payload+3);
    	    	  	  	  motor_RIGHT =  buffer[3] - 48;
    	          	  }
    	  }

        /* copy data to pbuf */
        /* send udp data */
        /* free pbuf */
        pbuf_free(p);
      }
}



err_t create_udp_socket(){
    err_t err = ERR_OK;
    ip4_addr_t destIPAddr;
    upcb = udp_new();

    if (upcb == NULL){
        return ERR_MEM;
    }
    //IP4_ADDR(&destIPAddr,192,168,1,54);
    IP4_ADDR(&destIPAddr,192,168,1,49);
   // upcb->local_port = 5001;
    //upcb->local_port = 4004; // Set our local port to 4004
    // Should bind to the local ip and port
    err = udp_bind(upcb,IP4_ADDR_ANY,5002);

    if (err != ERR_OK){
        return err;
    }
    // Connect to the other port
    //err = udp_connect(upcb,&destIPAddr,5001);
    err = udp_connect(upcb,&destIPAddr,5001);
    if (err != ERR_OK){
        return err;
    }

	//pp = pbuf_alloc(PBUF_TRANSPORT,5, PBUF_POOL);


    // Set the receive function
    udp_recv(upcb,udp_receive_callback_R,NULL);
    return err;
}


err_t create_udp_socket2(){
    err_t err = ERR_OK;
    ip4_addr_t destIPAddr;
    upcb2 = udp_new();
    upcb2->next = NULL;

    if (upcb2 == NULL){
        return ERR_MEM;
    }
    //IP4_ADDR(&destIPAddr,192,168,1,60);
    IP4_ADDR(&destIPAddr,192,168,1,48);
   // upcb->local_port = 5001;
    //upcb->local_port = 4004; // Set our local port to 4004
    // Should bind to the local ip and port
    err = udp_bind(upcb2,IP4_ADDR_ANY,5004);
    if (err != ERR_OK){
        return err;
    }
    // Connect to the other port
    err = udp_connect(upcb2,&destIPAddr,5001);
    if (err != ERR_OK){
        return err;
    }


    // Set the receive function
    udp_recv(upcb,udp_receive_callback_L,NULL);
    return err;
}






err_t send_msg_to_dest2(char in ){
    struct pbuf *p;
	uint8_t data[6]={0};

	printf("tc %d \n",in);
    //char ans[2];
   // int_to_char(ans,in);

	  data[0] = 'T';
	  data[1] = 'C';
	  data[2] = '=';
	  data[3] = in/10 + 48;
	  data[4] = in%10 + 48;
	  data[5] = '\r';

    /* allocate pbuf from pool*/
    p = pbuf_alloc(PBUF_TRANSPORT,6, PBUF_POOL);
    pbuf_dechain(p);

    if (p != NULL)
    {
        /* copy data to pbuf */
        pbuf_take(p, (char*)data, 6);

        /* send udp data */
        udp_send(upcb2, p);

        /* free pbuf */
        pbuf_free(p);
        return ERR_OK;
    }
    return ERR_OK;
}

//want to replace the original colling



err_t send_msg_to_dest(char in ){
        struct pbuf *p;
    	uint8_t data[7]={0};

        //char ans[2];
        //int_to_char(ans,in);

    	  data[0] = 'T';
    	  data[1] = 'C';
    	  data[2] = '=';
    	  data[3] = '-';
    	  data[4] = in/10 + 48;
    	  data[5] = in%10 + 48;
    	  data[6] = '\r';

        /* allocate pbuf from pool*/
        p = pbuf_alloc(PBUF_TRANSPORT,7, PBUF_POOL);
        pbuf_dechain(p);



        if (p != NULL)
        {
            /* copy data to pbuf */
            pbuf_take(p, (char*)data, 7);

            /* send udp data */
            udp_send(upcb, p);

            /* free pbuf */
            pbuf_free(p);
            return ERR_OK;
        }


    return ERR_MEM;
    printf("error allocate the pbuf\n");
}


err_t Start_Motor_1( void ){
        struct pbuf *p;
    	uint8_t data[5]={0};


    	  data[0] = 'M';
    	  data[1] = 'O';
    	  data[2] = '=';
    	  data[3] = '1';
    	  data[4] = '\r';

        /* allocate pbuf from pool*/
        p = pbuf_alloc(PBUF_TRANSPORT,5, PBUF_POOL);
        pbuf_dechain(p);



        if (p != NULL)
        {
            /* copy data to pbuf */
            pbuf_take(p, (char*)data, 5);

            /* send udp data */
            udp_send(upcb, p);

            /* free pbuf */
            pbuf_free(p);
            return ERR_OK;
        }


    return ERR_MEM;
    printf("error allocate the pbuf\n");
}


err_t ASK_Motor_1( void ){
        struct pbuf *p;
    	uint8_t data[3]={0};

    	  data[0] = 'M';
    	  data[1] = 'O';
    	  data[2] = '\r';

        /* allocate pbuf from pool*/
        p = pbuf_alloc(PBUF_TRANSPORT,3, PBUF_POOL);
        pbuf_dechain(p);



        if (p != NULL)
        {
            /* copy data to pbuf */
            pbuf_take(p, (char*)data, 3);

            /* send udp data */
            udp_send(upcb, p);

            /* free pbuf */
            pbuf_free(p);
            return ERR_OK;
        }


    return ERR_MEM;
    printf("error allocate the pbuf\n");
}


err_t ASK_Motor_2( void ){
        struct pbuf *p;
    	uint8_t data[3]={0};

    	  data[0] = 'M';
    	  data[1] = 'O';
    	  data[2] = '\r';

        /* allocate pbuf from pool*/
        p = pbuf_alloc(PBUF_TRANSPORT,3, PBUF_POOL);
        pbuf_dechain(p);



        if (p != NULL)
        {
            /* copy data to pbuf */
            pbuf_take(p, (char*)data, 3);

            /* send udp data */
            udp_send(upcb2, p);

            /* free pbuf */
            pbuf_free(p);
            return ERR_OK;
        }


    return ERR_MEM;
    printf("error allocate the pbuf\n");
}




err_t Start_Motor_2( void ){
        struct pbuf *p;
    	uint8_t data[5]={0};


    	  data[0] = 'M';
    	  data[1] = 'O';
    	  data[2] = '=';
    	  data[3] = '1';
    	  data[4] = '\r';

        /* allocate pbuf from pool*/
        p = pbuf_alloc(PBUF_TRANSPORT,5, PBUF_POOL);
        pbuf_dechain(p);



        if (p != NULL)
        {
            /* copy data to pbuf */
            pbuf_take(p, (char*)data, 5);

            /* send udp data */
            udp_send(upcb2, p);

            /* free pbuf */
            pbuf_free(p);
            return ERR_OK;
        }
        return ERR_OK;


    return ERR_MEM;
    printf("error allocate the pbuf\n");
}


/* USER CODE END 2 */

/**
  * LwIP initialization function
  */
void MX_LWIP_Init(void)
{
  /* IP addresses initialization */
  IP_ADDRESS[0] = 192;
  IP_ADDRESS[1] = 168;
  IP_ADDRESS[2] = 1;
  IP_ADDRESS[3] = 51;
  NETMASK_ADDRESS[0] = 255;
  NETMASK_ADDRESS[1] = 255;
  NETMASK_ADDRESS[2] = 255;
  NETMASK_ADDRESS[3] = 0;
  GATEWAY_ADDRESS[0] = 0;
  GATEWAY_ADDRESS[1] = 0;
  GATEWAY_ADDRESS[2] = 0;
  GATEWAY_ADDRESS[3] = 0;
  
  /* Initilialize the LwIP stack without RTOS */
  lwip_init();

  /* IP addresses initialization without DHCP (IPv4) */
  IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
  IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
  IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);

  /* add the network interface (IPv4/IPv6) without RTOS */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  /* Registers the default network interface */
  netif_set_default(&gnetif);

  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }

/* USER CODE BEGIN 3 */

  // Setup the udp port
  create_udp_socket2();
  create_udp_socket();
  printf("\rcreate udp socket\n");
}

void int_to_char(char ans[2],int num){

	ans[0] = num/10 + 48;
	ans[1] = num%10 + 48;

}


/* USER CODE END 3 */
}

#ifdef USE_OBSOLETE_USER_CODE_SECTION_4
/* Kept to help code migration. (See new 4_1, 4_2... sections) */
/* Avoid to use this user section which will become obsolete. */
/* USER CODE BEGIN 4 */
/* USER CODE END 4 */
#endif

/**
 * ----------------------------------------------------------------------
 * Function given to help user to continue LwIP Initialization
 * Up to user to complete or change this function ...
 * Up to user to call this function in main.c in while (1) of main(void) 
 *-----------------------------------------------------------------------
 * Read a received packet from the Ethernet buffers 
 * Send it to the lwIP stack for handling
 * Handle timeouts if LWIP_TIMERS is set and without RTOS
 * Handle the llink status if LWIP_NETIF_LINK_CALLBACK is set and without RTOS 
 */
void MX_LWIP_Process(void)
{
/* USER CODE BEGIN 4_1 */
 // ethernetif_set_link(&gnetif);
/* USER CODE END 4_1 */
  ethernetif_input(&gnetif);
  
/* USER CODE BEGIN 4_2 */
  //send_msg_to_dest();
 // printf("\rsend message\n");
/* USER CODE END 4_2 */  
  /* Handle timeouts */
  sys_check_timeouts();

/* USER CODE BEGIN 4_3 */

/* USER CODE END 4_3 */
}

#if defined ( __CC_ARM )  /* MDK ARM Compiler */
/**
 * Opens a serial device for communication.
 *
 * @param devnum device number
 * @return handle to serial device if successful, NULL otherwise
 */
sio_fd_t sio_open(u8_t devnum)
{
  sio_fd_t sd;

/* USER CODE BEGIN 7 */
  sd = 0; // dummy code
/* USER CODE END 7 */
	
  return sd;
}

/**
 * Sends a single character to the serial device.
 *
 * @param c character to send
 * @param fd serial device handle
 *
 * @note This function will block until the character can be sent.
 */
void sio_send(u8_t c, sio_fd_t fd)
{
/* USER CODE BEGIN 8 */
/* USER CODE END 8 */
}

/**
 * Reads from the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received - may be 0 if aborted by sio_read_abort
 *
 * @note This function will block until data can be received. The blocking
 * can be cancelled by calling sio_read_abort().
 */
u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;

/* USER CODE BEGIN 9 */
  recved_bytes = 0; // dummy code
/* USER CODE END 9 */	
  return recved_bytes;
}

/**
 * Tries to read from the serial device. Same as sio_read but returns
 * immediately if no data is available and never blocks.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received
 */
u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;

/* USER CODE BEGIN 10 */
  recved_bytes = 0; // dummy code
/* USER CODE END 10 */	
  return recved_bytes;
}
#endif /* MDK ARM Compiler */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
