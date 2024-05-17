/**
  ******************************************************************************
  * @file    WIZnet MDK5 Project Template  ../main.c 
  * @author  WIZnet Software Team
  * @version V1.0.0
  * @date    2018-09-25
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2018 WIZnet H.K. Ltd.</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "w5500.h"
#include <stdio.h>

#define _W5500_SPI_VDM_OP_          0x00
#define _W5500_SPI_FDM_OP_LEN1_     0x01
#define _W5500_SPI_FDM_OP_LEN2_     0x02
#define _W5500_SPI_FDM_OP_LEN4_     0x03

////////////////////////////////////////////////////

uint8_t  WIZCHIP_READ(uint32_t AddrSel)
{
   uint8_t ret;

   WIZCHIP_CRITICAL_ENTER();
   WIZCHIP.CS._select();

#if( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_))

   #if  ( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_ )
   	   AddrSel |= (_W5500_SPI_READ_ | _W5500_SPI_VDM_OP_);
   #elif( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_ )
   	   AddrSel |= (_W5500_SPI_READ_ | _W5500_SPI_FDM_OP_LEN1_);
   #else
      #error "Unsupported _WIZCHIP_IO_SPI_ in W5500 !!!"
   #endif

   WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
   WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
   WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
   ret = WIZCHIP.IF.SPI._read_byte();
#elif ( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_BUS_) )

   #if  (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_DIR_)

   #elif(_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_)

   #else
      #error "Unsupported _WIZCHIP_IO_MODE_BUS_ in W5500 !!!"
   #endif
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5000. !!!"   
#endif

   WIZCHIP.CS._deselect();
   WIZCHIP_CRITICAL_EXIT();
   return ret;
}

void     WIZCHIP_WRITE(uint32_t AddrSel, uint8_t wb )
{
    WIZCHIP_CRITICAL_ENTER();
    WIZCHIP.CS._select();

#if( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_))

   #if  ( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_ )
   	   AddrSel |= (_W5500_SPI_WRITE_ | _W5500_SPI_VDM_OP_);
   #elif( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_ )
   	   AddrSel |= (_W5500_SPI_WRITE_ | _W5500_SPI_FDM_OP_LEN1_);
   #else
      #error "Unsupported _WIZCHIP_IO_SPI_ in W5500 !!!"
   #endif

   WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
   WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
   WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
   WIZCHIP.IF.SPI._write_byte(wb);

#elif ( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_BUS_) )

   #if  (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_DIR_)

   #elif(_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_)

   #else
      #error "Unsupported _WIZCHIP_IO_MODE_BUS_ in W5500 !!!"
   #endif
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5500. !!!"
#endif

   WIZCHIP.CS._deselect();
   WIZCHIP_CRITICAL_EXIT();
}
         
void     WIZCHIP_READ_BUF (uint32_t AddrSel, uint8_t* pBuf, uint16_t len)
{
   uint16_t i = 0;
//   uint16_t j = 0;
   WIZCHIP_CRITICAL_ENTER();
   WIZCHIP.CS._select();

#if( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_))

   #if  ( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_ )
      AddrSel |= (_W5500_SPI_READ_ | _W5500_SPI_VDM_OP_);
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
      for(i = 0; i < len; i++)
        pBuf[i] = WIZCHIP.IF.SPI._read_byte();
   #elif( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_ )
      AddrSel |= (_W5500_SPI_READ_ | _W5500_SPI_FDM_OP_LEN4_);
      for(i = 0; i < len/4; i++, j)
      {
         WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
         WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
         WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
         pBuf[i*4]   = WIZCHIP.IF.SPI._read_byte();
         pBuf[i*4+1] = WIZCHIP.IF.SPI._read_byte();
         pBuf[i*4+2] = WIZCHIP.IF.SPI._read_byte();            
         pBuf[i*4+3] = WIZCHIP.IF.SPI._read_byte();            
         AddrSel = WIZCHIP_OFFSET_INC(AddrSel,4);
      }
      len %= 4;      // for the rest data
      // M20131220 : remove for loop
      i *= 4;        
      if(len >= 2)
      {
         AddrSel -= 1;  // change _W5500_SPI_FDM_OP_LEN4_ to _W5500_SPI_FDM_OP_LEN2_

       //for(j = 0; j < len/2 ; j++)
         {
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
            pBuf[i]   = WIZCHIP.IF.SPI._read_byte();
            pBuf[i+1] = WIZCHIP.IF.SPI._read_byte();
            i += 2;
            AddrSel = WIZCHIP_OFFSET_INC(AddrSel,2);
         }
      }
      len %= 2;
      if(len)
      {
        AddrSel -= 1;  // change _W5500_SPI_FDM_OP_LEN2_ to _W5500_SPI_FDM_OP_LEN1_
        WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
        WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
        WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
        pBuf[i]   = WIZCHIP.IF.SPI._read_byte();
      }      
   #else
      #error "Unsupported _WIZCHIP_IO_MODE_SPI_ in W5500 !!!"
   #endif

#elif ( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_BUS_) )

   #if  (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_DIR_)

   #elif(_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_)

   #else
      #error "Unsupported _WIZCHIP_IO_MODE_BUS_ in W5500 !!!"
   #endif
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5500. !!!!"
#endif

   WIZCHIP.CS._deselect();
   WIZCHIP_CRITICAL_EXIT();
}

void     WIZCHIP_WRITE_BUF(uint32_t AddrSel, uint8_t* pBuf, uint16_t len)
{
   uint16_t i = 0;
//   uint16_t j = 0;
   WIZCHIP_CRITICAL_ENTER();
   WIZCHIP.CS._select();

#if( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_))

   #if  ( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_ )
      AddrSel |= (_W5500_SPI_WRITE_ | _W5500_SPI_VDM_OP_);
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
      for(i = 0; i < len; i++)
         WIZCHIP.IF.SPI._write_byte(pBuf[i]);
   #elif( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_ )
      AddrSel |= (_W5500_SPI_WRITE_ | _W5500_SPI_FDM_OP_LEN4_);
      for(i = 0; i < len/4; i++, j)
      {
         WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
         WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
         WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
         WIZCHIP.IF.SPI._write_byte(pBuf[i*4]  );
         WIZCHIP.IF.SPI._write_byte(pBuf[i*4+1]);
         WIZCHIP.IF.SPI._write_byte(pBuf[i*4+2]);            
         WIZCHIP.IF.SPI._write_byte(pBuf[i*4+3]);            
         AddrSel = WIZCHIP_OFFSET_INC(AddrSel,4);
      }
      len %= 4;      // for the rest data
      // M20131220 : Remove for loop
      i *= 4;
      if(len >= 2)
      {
         AddrSel -= 1;  // change _W5500_SPI_FDM_OP_LEN4_ to _W5500_SPI_FDM_OP_LEN2_

       //for(j = 0; j < len/2 ; j++)
         {
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
            WIZCHIP.IF.SPI._write_byte(pBuf[i]  );
            WIZCHIP.IF.SPI._write_byte(pBuf[i+1]);
            i += 2;
            AddrSel = WIZCHIP_OFFSET_INC(AddrSel, 2);
         }
         len %= 2;
         if(len)
         {
            AddrSel -= 1;  // change _W5500_SPI_FDM_OP_LEN2_ to _W5500_SPI_FDM_OP_LEN1_
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
            WIZCHIP.IF.SPI._write_byte(pBuf[i]);
         }      
      }
   #else
      #error "Unsupported _WIZCHIP_IO_SPI_ in W5500 !!!"
   #endif

#elif ( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_BUS_) )

   #if  (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_DIR_)

   #elif(_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_)

   #else
      #error "Unsupported _WIZCHIP_IO_MODE_BUS_ in W5500 !!!"
   #endif
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5500. !!!!"
#endif

   WIZCHIP.CS._deselect();
   WIZCHIP_CRITICAL_EXIT();
}


uint16_t getSn_TX_FSR(uint8_t sn)
{
   uint16_t val=0,val1=0;
   do
   {
      val1 = WIZCHIP_READ(Sn_TX_FSR(sn));
      val1 = (val1 << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_TX_FSR(sn),1));
      if (val1 != 0)
      {
        val = WIZCHIP_READ(Sn_TX_FSR(sn));
        val = (val << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_TX_FSR(sn),1));
      }
   }while (val != val1);
   return val;
}


uint16_t getSn_RX_RSR(uint8_t sn)
{
   uint16_t val=0,val1=0;
   do
   {
      val1 = WIZCHIP_READ(Sn_RX_RSR(sn));
      val1 = (val1 << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_RX_RSR(sn),1));		  
      if (val1 != 0)
      {
        val = WIZCHIP_READ(Sn_RX_RSR(sn));
        val = (val << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_RX_RSR(sn),1));
      }
   }while (val != val1);
   return val;
}

void wiz_send_data(uint8_t sn, uint8_t *wizdata, uint16_t len)
{
   uint16_t ptr = 0;
   uint32_t addrsel = 0;
   if(len == 0)  return;
   ptr = getSn_TX_WR(sn);
   //M20140501 : implict type casting -> explict type casting
   //addrsel = (ptr << 8) + (WIZCHIP_TXBUF_BLOCK(sn) << 3);
   addrsel = ((uint32_t)ptr << 8) + (WIZCHIP_TXBUF_BLOCK(sn) << 3);
   //
   WIZCHIP_WRITE_BUF(addrsel,wizdata, len);
   
   ptr += len;
   setSn_TX_WR(sn,ptr);
}

void wiz_recv_data(uint8_t sn, uint8_t *wizdata, uint16_t len)
{
   uint16_t ptr = 0;
   uint32_t addrsel = 0;
   
   if(len == 0) return;
   ptr = getSn_RX_RD(sn);
   //M20140501 : implict type casting -> explict type casting
   //addrsel = ((ptr << 8) + (WIZCHIP_RXBUF_BLOCK(sn) << 3);
   addrsel = ((uint32_t)ptr << 8) + (WIZCHIP_RXBUF_BLOCK(sn) << 3);
   //
   WIZCHIP_READ_BUF(addrsel, wizdata, len);
   ptr += len;
   
   setSn_RX_RD(sn,ptr);
}


void wiz_recv_ignore(uint8_t sn, uint16_t len)
{
   uint16_t ptr = 0;
   ptr = getSn_RX_RD(sn);
   ptr += len;
   setSn_RX_RD(sn,ptr);
}

