#define KXVER 3

#include "k.h"
#include "ta_libc.h"
#include <float.h>



K Initialize() {

  TA_RetCode retCode;
  retCode = TA_Initialize();
    
  if( retCode != TA_SUCCESS )
    R krr("tainiterror");

}


K Shutdown() {
  TA_Shutdown();
}




/*
ADX - Average Directional Movement Index

Input parameters:
  high - array of floats
  low - array of floats
  close - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInTimePeriod
Returns:
  array of floats
*/

K ADX(K high, K low, K close, K startIdx, K endIdx, K period) {

  int typeq = high->t;
  int lenq = high->n;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    highPrice[lenq];
  TA_Real    lowPrice[lenq];
  TA_Real    closePrice[lenq];
  TA_Real    out[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    highPrice[i] = (double) kF(high)[i];
    lowPrice[i] = (double) kF(low)[i];
    closePrice[i] = (double) kF(close)[i];
  }

  TA_RetCode retCode = TA_ADX(startIdx->i, endIdx->i,
    &highPrice[0],
    &lowPrice[0],
    &closePrice[0],
    period->i,
    &outBeg, &outNbElement, &out[0] );


  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }
  
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[i];
  }
  

  /*
  int ii = lenq-outBeg-3;
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[ii];
    ii--;
  }
  */

  return outReal;

}




/*
ATR - Average True Range

Input parameters:
  high - array of floats
  low - array of floats
  close - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInTimePeriod

Returns:
  array of floats
*/

K ATR(K high, K low, K close, K startIdx, K endIdx, K period) {

  int typeq = high->t;
  int lenq = high->n;
  // int allocationSize = endIdx->i - startIdx->i + 1;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    highPrice[lenq];
  TA_Real    lowPrice[lenq];
  TA_Real    closePrice[lenq];
  TA_Real    out[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    highPrice[i] = (double) kF(high)[i];
    lowPrice[i] = (double) kF(low)[i];
    closePrice[i] = (double) kF(close)[i];
  }

  TA_RetCode retCode = TA_ATR(startIdx->i, endIdx->i,
    &highPrice[0],
    &lowPrice[0],
    &closePrice[0],
    period->i,
    &outBeg, &outNbElement, &out[0] );

  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }
  
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[i];
  }

  /*
  int ii = lenq-outBeg-1;
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[ii];
    ii--;
  }
  */
  
  return outReal;

}




/*
BBANDS - Bollinger Bands

Input parameters:
  inReal - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInTimePeriod
  optInNbDevUp
  optInNbDevDn
  optInMAType
Returns:
  array of floats
*/

K BBANDS(K inReal, K startIdx, K endIdx, K period, K optInNbDevUp, K optInNbDevDn, K outSelect) {

  int typeq = inReal->t;
  int lenq = inReal->n;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    inPrice[lenq];
  TA_Real    outRealUpperBand[lenq];
  TA_Real    outRealMiddleBand[lenq];
  TA_Real    outRealLowerBand[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    inPrice[i] = (double) kF(inReal)[i];
  }

  TA_RetCode retCode = TA_BBANDS(startIdx->i, endIdx->i,
    &inPrice[0],
    period->i,
    optInNbDevUp->i,
    optInNbDevDn->i,
    TA_MAType_SMA,
    &outBeg, &outNbElement, &outRealUpperBand[0], &outRealMiddleBand[0], &outRealLowerBand[0] );

  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }

  if(outSelect->i == 1){
    for(i=outBeg;i<lenq;i++){
      kF(outReal)[i] = outRealUpperBand[i];
    }
  } else if(outSelect->i == 2){
    for(i=outBeg;i<lenq;i++){
      kF(outReal)[i] = outRealMiddleBand[i];
    }
  } else {
    for(i=outBeg;i<lenq;i++){
      kF(outReal)[i] = outRealLowerBand[i];
    }
  }

  return outReal;

}



/*
BETA - Beta

Input parameters:
  inReal0 - array of floats
  inReal1 - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInTimePeriod
Returns:
  array of floats
*/

K BETA(K inReal0, K inReal1, K startIdx, K endIdx, K period) {

  int typeq = inReal0->t;
  int lenq = inReal0->n;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    inPrice0[lenq];
  TA_Real    inPrice1[lenq];
  TA_Real    out[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    inPrice0[i] = (double) kF(inReal0)[i];
    inPrice1[i] = (double) kF(inReal1)[i];
  }

  TA_RetCode retCode = TA_BETA(startIdx->i, endIdx->i,
    &inPrice0[0],
    &inPrice1[0],
    period->i,
    &outBeg, &outNbElement, &out[0] );

  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }
        
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[i];
  }

  /*
  int ii = lenq-outBeg-3;
  for(i=outBeg+2;i<lenq;i++){
    kF(outReal)[i] = out[ii];
    ii--;
  }
  */

  return outReal;

}




/*
BOP - Balance Of Power

Input parameters:
  open - array of floats
  high - array of floats
  low - array of floats
  close - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
Returns:
  array of floats
*/

K BOP(K open, K high, K low, K close, K startIdx, K endIdx) {

  int typeq = open->t;
  int lenq = open->n;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    openPrice[lenq];
  TA_Real    highPrice[lenq];
  TA_Real    lowPrice[lenq];
  TA_Real    closePrice[lenq];
  TA_Real    out[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    openPrice[i] = (double) kF(open)[i];
    highPrice[i] = (double) kF(low)[i];
    lowPrice[i] = (double) kF(low)[i];
    closePrice[i] = (double) kF(close)[i];
  }

  TA_RetCode retCode = TA_BOP(startIdx->i, endIdx->i,
    &openPrice[0],
    &highPrice[0],
    &lowPrice[0],
    &closePrice[0],
    &outBeg, &outNbElement, &out[0] );

  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }
        
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[i];
  }

  /*
  int ii = lenq-outBeg-3;
  for(i=outBeg+2;i<lenq;i++){
    kF(outReal)[i] = out[ii];
    ii--;
  }
  */

  return outReal;

}




/*
CCI - Commodity Channel Index

Input parameters:
  high - array of floats
  low - array of floats
  close - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInTimePeriod
Returns:
  array of floats
*/

K CCI(K high, K low, K close, K startIdx, K endIdx, K period) {

  int typeq = high->t;
  int lenq = high->n;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    highPrice[lenq];
  TA_Real    lowPrice[lenq];
  TA_Real    closePrice[lenq];
  TA_Real    out[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    highPrice[i] = (double) kF(high)[i];
    lowPrice[i] = (double) kF(low)[i];
    closePrice[i] = (double) kF(close)[i];
  }

  TA_RetCode retCode = TA_CCI(startIdx->i, endIdx->i,
    &highPrice[0],
    &lowPrice[0],
    &closePrice[0],
    period->i,
    &outBeg, &outNbElement, &out[0] );

  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }
        
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[i];
  }

  /*
  int ii = lenq-outBeg-3;
  for(i=outBeg+2;i<lenq;i++){
    kF(outReal)[i] = out[ii];
    ii--;
  }
  */

  return outReal;

}




            


/*
CORREL - Pearson's Correlation Coefficient (r)

Input parameters:
  inReal0 - array of floats
  inReal1 - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInTimePeriod
Returns:
  array of floats
*/

K CORREL(K inReal0, K inReal1, K startIdx, K endIdx, K period) {

  int typeq = inReal0->t;
  int lenq = inReal0->n;

  /*
  if (typeq != 9){
    R krr("qtype");
  }
  */

  TA_Real    inPrice0[lenq];
  TA_Real    inPrice1[lenq];
  TA_Real    out[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    inPrice0[i] = (double) kF(inReal0)[i];
    inPrice1[i] = (double) kF(inReal1)[i];
  }

  TA_RetCode retCode = TA_CORREL(startIdx->i, endIdx->i,
    &inPrice0[0],
    &inPrice1[0],
    period->i,
    &outBeg, &outNbElement, &out[0] );

  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }
        
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[i];
  }

  /*
  int ii = lenq-outBeg-3;
  for(i=outBeg+2;i<lenq;i++){
    kF(outReal)[i] = out[ii];
    ii--;
  }
  */

  return outReal;

}



/*
DX - Directional Movement Index

Input parameters:
  high - array of floats
  low - array of floats
  close - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInTimePeriod
Returns:
  array of floats
*/

K DX(K high, K low, K close, K startIdx, K endIdx, K period) {

  int typeq = high->t;
  int lenq = high->n;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    highPrice[lenq];
  TA_Real    lowPrice[lenq];
  TA_Real    closePrice[lenq];
  TA_Real    out[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    highPrice[i] = (double) kF(high)[i];
    lowPrice[i] = (double) kF(low)[i];
    closePrice[i] = (double) kF(close)[i];
  }

  TA_RetCode retCode = TA_DX(startIdx->i, endIdx->i,
    &highPrice[0],
    &lowPrice[0],
    &closePrice[0],
    period->i,
    &outBeg, &outNbElement, &out[0] );


  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }
        
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[i];
  }

  /*
  int ii = lenq-outBeg-3;
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[ii];
    ii--;
  }
  */

  return outReal;

}





/*
EMA - Exponential Moving Average

Input parameters:
  inReal - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInTimePeriod
Returns:
  array of floats

UNTESTED

*/
K EMA(K inReal, K startIdx, K endIdx, K period) {

  int typeq = inReal->t;
  int lenq = inReal->n;
  // int allocationSize = endIdx->i - startIdx->i + 1;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    closePrice[lenq];
  TA_Real    out[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    closePrice[i] = (double) kF(inReal)[i];
  }

  TA_RetCode retCode = TA_EMA(startIdx->i, endIdx->i,
    &closePrice[0],
    period->i,
    &outBeg, &outNbElement, &out[0] );

  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }
        
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[i];
  }

  /*
  int ii = lenq-outBeg-3;
  for(i=outBeg+2;i<lenq;i++){
    kF(outReal)[i] = out[ii];
    ii--;
  }
  */
  
  return outReal;

}



/*
LINEARREG - Linear Regression

Input parameters:
  inReal - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInTimePeriod
Returns:
  array of floats
*/
K LINEARREG(K inReal, K startIdx, K endIdx, K period) {

  int typeq = inReal->t;
  int lenq = inReal->n;
  // int allocationSize = endIdx->i - startIdx->i + 1;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    closePrice[lenq];
  TA_Real    out[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    closePrice[i] = (double) kF(inReal)[i];
  }

  TA_RetCode retCode = TA_LINEARREG(startIdx->i, endIdx->i,
    &closePrice[0],
    period->i,
    &outBeg, &outNbElement, &out[0] );

  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }
        
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[i];
  }

  /*
  int ii = lenq-outBeg-3;
  for(i=outBeg+2;i<lenq;i++){
    kF(outReal)[i] = out[ii];
    ii--;
  }
  */
  
  return outReal;

}



/*
MACD - Moving Average Convergence/Divergence

Input parameters:
  inReal - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInFastPeriod
  optInSlowPeriod
  optInSignalPeriod
  outSelect
Returns:
  outMACD outMACDSignal outMACDHist
*/

K MACD(K inReal, K startIdx, K endIdx, K optInFastPeriod, K optInSlowPeriod, K optInSignalPeriod, K outSelect) {

  int typeq = inReal->t;
  int lenq = inReal->n;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    closePrice[lenq];
  TA_Real    outMACD[lenq];
  TA_Real    outMACDSignal[lenq];
  TA_Real    outMACDHist[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    closePrice[i] = (double) kF(inReal)[i];
  }

  TA_RetCode retCode = TA_MACD(startIdx->i, endIdx->i,
    &closePrice[0],
    optInFastPeriod->i,
    optInSlowPeriod->i,
    optInSignalPeriod->i,
    &outBeg, &outNbElement, &outMACD[0], &outMACDSignal[0], &outMACDHist[0] );

  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }


  if(outSelect->i == 1){
    for(i=outBeg;i<lenq;i++){
      kF(outReal)[i] = outMACDSignal[i];
    }
  } else if(outSelect->i == 2) {
    for(i=outBeg;i<lenq;i++){
      kF(outReal)[i] = outMACDHist[i];
    }
  } else {
    for(i=outBeg;i<lenq;i++){
      kF(outReal)[i] = outMACD[i];
    }
  }

  return outReal;

}



/*
MACDFIX - Moving Average Convergence/Divergence Fix 12/26

Input parameters:
  inReal - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInSignalPeriod
  outSelect
Returns:
  outMACD outMACDSignal outMACDHist
*/

K MACDFIX(K inReal, K startIdx, K endIdx, K optInSignalPeriod, K outSelect) {

  int typeq = inReal->t;
  int lenq = inReal->n;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    closePrice[lenq];
  TA_Real    outMACD[lenq];
  TA_Real    outMACDSignal[lenq];
  TA_Real    outMACDHist[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    closePrice[i] = (double) kF(inReal)[i];
  }

  TA_RetCode retCode = TA_MACDFIX(startIdx->i, endIdx->i,
    &closePrice[0],
    optInSignalPeriod->i,
    &outBeg, &outNbElement, &outMACD[0], &outMACDSignal[0], &outMACDHist[0] );

  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }

  int ii = lenq-outBeg-1;
  if(outSelect->i == 1){
    for(i=outBeg;i<lenq;i++){
      kF(outReal)[i] = outMACDSignal[i];
    }
  } else if(outSelect->i == 2) {
    for(i=outBeg;i<lenq;i++){
      kF(outReal)[i] = outMACDHist[i];
    }
  } else {
    for(i=outBeg;i<lenq;i++){
      kF(outReal)[i] = outMACD[i];
    }
  }

  return outReal;

}



/*
MFI - Money Flow Index

Input parameters:
  high - array of floats
  low - array of floats
  close - array of floats
  volume - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInTimePeriod
Returns:
  array of floats
*/


K MFI(K high, K low, K close, K volume, K startIdx, K endIdx, K period) {

  int typeq = high->t;
  int lenq = high->n;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    highPrice[lenq];
  TA_Real    lowPrice[lenq];
  TA_Real    closePrice[lenq];
  TA_Real    volumeIn[lenq];
  TA_Real    out[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    highPrice[i] = (double) kF(high)[i];
    lowPrice[i] = (double) kF(low)[i];
    closePrice[i] = (double) kF(close)[i];
    volumeIn[i] = (double) kF(volume)[i];
  }

  TA_RetCode retCode = TA_MFI(startIdx->i, endIdx->i,
    &highPrice[0],
    &lowPrice[0],
    &closePrice[0],
    &volumeIn[0],
    period->i,
    &outBeg, &outNbElement, &out[0] );

  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }
        
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[i];
  }
  

  /*
  int ii = lenq-outBeg-3;
  for(i=outBeg+2;i<lenq;i++){
    kF(outReal)[i] = out[ii];
    ii--;
  }
  */

  return outReal;

}





/*
MINUS_DI - Minus Directional Indicator

Input parameters:
  high - array of floats
  low - array of floats
  close - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInTimePeriod
Returns:
  array of floats
*/

K MINUS_DI(K high, K low, K close, K startIdx, K endIdx, K period) {

  int typeq = high->t;
  int lenq = high->n;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    highPrice[lenq];
  TA_Real    lowPrice[lenq];
  TA_Real    closePrice[lenq];
  TA_Real    out[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    highPrice[i] = (double) kF(high)[i];
    lowPrice[i] = (double) kF(low)[i];
    closePrice[i] = (double) kF(close)[i];
  }

  TA_RetCode retCode = TA_MINUS_DI(startIdx->i, endIdx->i,
    &highPrice[0],
    &lowPrice[0],
    &closePrice[0],
    period->i,
    &outBeg, &outNbElement, &out[0] );


  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }
        
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[i];
  }
                

  /*
  int ii = lenq-outBeg-3;
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[ii];
    ii--;
  }
  */

  return outReal;

}




/*
MOM - Momentum

Input parameters:
  inReal - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInTimePeriod
Returns:
  array of floats
*/


K MOM(K inReal, K startIdx, K endIdx, K period) {

  int typeq = inReal->t;
  int lenq = inReal->n;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    inPrice[lenq];
  TA_Real    out[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    inPrice[i] = (double) kF(inReal)[i];
  }

  TA_RetCode retCode = TA_MOM(startIdx->i, endIdx->i,
    &inPrice[0],
    period->i,
    &outBeg, &outNbElement, &out[0] );

  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }
        
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[i];
  }
                

  /*
  int ii = lenq-outBeg-3;
  for(i=outBeg+2;i<lenq;i++){
    kF(outReal)[i] = out[ii];
    ii--;
  }
  */

  return outReal;

}




/*
PLUS_DI - Plus Directional Indicator

Input parameters:
  high - array of floats
  low - array of floats
  close - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInTimePeriod
Returns:
  array of floats
*/

K PLUS_DI(K high, K low, K close, K startIdx, K endIdx, K period) {

  int typeq = high->t;
  int lenq = high->n;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    highPrice[lenq];
  TA_Real    lowPrice[lenq];
  TA_Real    closePrice[lenq];
  TA_Real    out[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    highPrice[i] = (double) kF(high)[i];
    lowPrice[i] = (double) kF(low)[i];
    closePrice[i] = (double) kF(close)[i];
  }

  TA_RetCode retCode = TA_PLUS_DI(startIdx->i, endIdx->i,
    &highPrice[0],
    &lowPrice[0],
    &closePrice[0],
    period->i,
    &outBeg, &outNbElement, &out[0] );


  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }
        
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[i];
  }
                

  /*
  int ii = lenq-outBeg-3;
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[ii];
    ii--;
  }
  */

  return outReal;

}




/*
RSI - Relative Strength Index

Input parameters:
  inReal - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInTimePeriod

Returns:
  array of floats
*/

K RSI(K inReal, K startIdx, K endIdx, K period) {

  int typeq = inReal->t;
  int lenq = inReal->n;
  // int allocationSize = endIdx->i - startIdx->i + 1;
  
  if (typeq != 9) {
    R krr("qtype");
  }
  
  TA_Real    closePrice[lenq];
  TA_Real    out[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;
  
  int i;
  for(i=0;i<lenq;i++){
    closePrice[i] = (double) kF(inReal)[i];
  }
  
  TA_RetCode retCode = TA_RSI(startIdx->i, endIdx->i,
    &closePrice[0],
    period->i,
    &outBeg, &outNbElement, &out[0] );
  
  K outReal = ktn(KF, lenq);
  
  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }
        
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[i];
  }
  
  
  /*
  int ii = lenq-outBeg-1;
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[ii];
    ii--;
  }
  */
  
  return outReal;
  
}




/*
SAR - Parabolic SAR

Input parameters:
  high - array of floats
  low - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInAcceleration
  optInMaximum
Returns:
  array of floats
*/

K SAR(K high, K low, K startIdx, K endIdx, K optInAcceleration, K optInMaximum) {

  int typeq = high->t;
  int lenq = low->n;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    highPrice[lenq];
  TA_Real    lowPrice[lenq];
  TA_Real    out[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    highPrice[i] = (double) kF(high)[i];
    lowPrice[i] = (double) kF(low)[i];
  }

  TA_RetCode retCode = TA_SAR(startIdx->i, endIdx->i,
    &highPrice[0],
    &lowPrice[0],
    optInAcceleration->i,
    optInMaximum->i,
    &outBeg, &outNbElement, &out[0] );

  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }
        
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[i];
  }
                
  
  /*
  int ii = lenq-outBeg-3;
  for(i=outBeg+2;i<lenq;i++){
    kF(outReal)[i] = out[ii];
    ii--;
  }
  */

  return outReal;

}




/*
SMA - Simple Moving Average

Input parameters:
  inReal - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInTimePeriod

Returns:
  array of floats
*/

K SMA(K inReal, K startIdx, K endIdx, K period) {

  int typeq = inReal->t;
  int lenq = inReal->n;
  // int allocationSize = endIdx->i - startIdx->i + 1;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    closePrice[lenq];
  TA_Real    out[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    closePrice[i] = (double) kF(inReal)[i];
  }

  TA_RetCode retCode = TA_MA(startIdx->i, endIdx->i,
    &closePrice[0],
    period->i,TA_MAType_SMA,
    &outBeg, &outNbElement, &out[0] );

  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }
        
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[i];
  }
                

  /*
  int ii = lenq-outBeg-3;
  for(i=outBeg+2;i<lenq;i++){
    kF(outReal)[i] = out[ii];
    ii--;
  }
  */

  return outReal;

}



/*
STDDEV - Standard Deviation

Input parameters:
  inReal - array of floats
  startIdx - start index for input data
  endIdx - end index for input data
  optInTimePeriod
  optInNbDev

Returns:
  array of floats
*/

K STDDEV(K inReal, K startIdx, K endIdx, K period, K optInNbDev) {

  int typeq = inReal->t;
  int lenq = inReal->n;
  // int allocationSize = endIdx->i - startIdx->i + 1;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    closePrice[lenq];
  TA_Real    out[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    closePrice[i] = (double) kF(inReal)[i];
  }

  TA_RetCode retCode = TA_STDDEV(startIdx->i, endIdx->i,
    &closePrice[0],
    period->i,
    optInNbDev->f,
    &outBeg, &outNbElement, &out[0] );

  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }
        
  for(i=outBeg;i<lenq;i++){
    kF(outReal)[i] = out[i];
  }
                

  /*
  int ii = lenq-outBeg-3;
  for(i=outBeg+2;i<lenq;i++){
    kF(outReal)[i] = out[ii];
    ii--;
  }
  */

  return outReal;

}





/*
STOCH - Stochastic

Input parameters:
  high - array of floats
  low - array of floats
  close - array of floats 
  startIdx - start index for input data
  endIdx - end index for input data
  optInFastK_Period
  optInSlowK_Period
  optInSlowK_MAType
  optInSlowD_Period
  optInSlowD_MAType
Returns:
  outSlowK outSlowD
*/

K STOCH(K high, K low, K close, K startIdx, K endIdx, K periodFastK, K periodSlow, K outSelect) {

  int typeq = high->t;
  int lenq = high->n;

  if (typeq != 9) {
    R krr("qtype");
  }

  TA_Real    highPrice[lenq];
  TA_Real    lowPrice[lenq];
  TA_Real    closePrice[lenq];
  TA_Real    outSlowK[lenq];
  TA_Real    outSlowD[lenq];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  int i;
  for(i=0;i<lenq;i++){
    highPrice[i] = (double) kF(high)[i];
    lowPrice[i] = (double) kF(low)[i];
    closePrice[i] = (double) kF(close)[i];
  }

  TA_RetCode retCode = TA_STOCH(startIdx->i, endIdx->i,
    &highPrice[0],
    &lowPrice[0],
    &closePrice[0],
    periodFastK->i,
    periodSlow->i,
    TA_MAType_SMA,
    periodSlow->i,
    TA_MAType_SMA,
    &outBeg, &outNbElement, &outSlowK[0], &outSlowD[0] );


  K outReal = ktn(KF, lenq);

  // Set Defaults
  for(i=0;i<outBeg;i++){
    kF(outReal)[i] = nf;
  }

  if(outSelect->i == 1){
    for(i=outBeg;i<lenq;i++){
      kF(outReal)[i] = outSlowK[i];
    }
  } else {
    for(i=outBeg;i<lenq;i++){
      kF(outReal)[i] = outSlowD[i];
    }
  }

  return outReal;

}




