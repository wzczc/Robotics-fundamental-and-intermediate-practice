/**
 * @file      TA0InputCapture.h
 * @brief     Initialize Timer A0
 * @details   Use Timer A0 in capture mode to request interrupts on rising
 * edge of P7.3 (TA0CCP0) and call a user function. 
 * Period measurement with units of 0.083 usec
 * @version   V1.0
 * @author    Valvano
 * @copyright Copyright 2017 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      July 23, 2017
 ******************************************************************************/

/* This example accompanies the books
   "Embedded Systems: Introduction to the MSP432 Microcontroller",
   ISBN: 978-1512185676, Jonathan Valvano, copyright (c) 2017

   "Embedded Systems: Real-Time Interfacing to the MSP432 Microcontroller",
   ISBN: 978-1514676585, Jonathan Valvano, copyright (c) 2017

   "Embedded Systems: Real-Time Operating Systems for ARM Cortex-M Microcontrollers",
   ISBN: 978-1466468863, Jonathan Valvano, copyright (c) 2017

 Copyright 2017 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
 */

// external signal connected to P7.3 (TA0CCP0) (trigger on both edges)


#ifndef TA0INPUTCAPTURE_H_
#define TA0INPUTCAPTURE_H_


/**
 * Initialize Timer A0 in edge time mode to request interrupts on
 * both edges of P7.3 (TA0CCP0).  The interrupt service routine
 * acknowledges the interrupt and calls a user function.
 * @param task is a pointer to a user function called when edge occurs<br>
 *        parameter is 16-bit up-counting timer value when edge occurred (units of 0.083 usec)
 * @return none
 * @note  Assumes low-speed subsystem master clock is 12 MHz
 * @brief  Initialize Timer A0
 */
void TimerA0Capture_Init(void(*task)(uint16_t time));

#endif /* TA0INPUTCAPTURE_H_ */
