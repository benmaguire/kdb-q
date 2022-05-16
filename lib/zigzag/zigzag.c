#define KXVER 3
#include"k.h"
#include <stdio.h>
#include <math.h>

#define PEAK 1
#define VALLEY -1


K _identify_initial_pivot(K x, double upthresh, double downthresh)
{

  int typeq = x->t;
  int lenq = x->n;
  
  double x_0 = kF(x)[0];
  double max_x = x_0;
  double max_t = 0;
  double min_x = x_0;
  double min_t = 0;
  
  double x_t;
  int t_n;
  
  int i;
  for(i=1;i<lenq;i++){
    x_t = kF(x)[i];
    
    if(x_t / min_x >= upthresh){
      if(min_t == 0){
        return ki(VALLEY);
      } else {
        return ki(PEAK);
      }
    }
    
    if(x_t / max_x <= downthresh){
      if(max_t == 0) {
        return ki(PEAK);
      } else {
        return ki(VALLEY);
      }
    }
    
    if(x_t > max_x){
      max_x = x_t;
      max_t = i;
    }
    
    if(x_t < min_x){
      min_x = x_t;
      min_t = i;
    }
    
  }
  
  t_n = lenq-1;
  if(x_0 < kF(x)[t_n]) {
    return ki(VALLEY);
  } else {
    return ki(PEAK);
  }
  
  return ki(0);
}




K peak_valley_pivots(K x,K up_thresh,K down_thresh,K firstp)
{
  
  int typeq = x->t;
  int lenq = x->n;
  
  if(lenq == 0){
    R krr("zlength");
  }
  
  // The down_thresh must be negative
  if (down_thresh->f > 0) {
    //printf("%f\n", down_thresh->f);
    R krr("thresh");
  }
  
  double upthresh = up_thresh->f+1;
  double downthresh = down_thresh->f+1;
  
  K initial_pivot = _identify_initial_pivot(x, upthresh, downthresh);
  
  int t_n = lenq;
  K pivots = ktn(0, lenq);
  
  // Set Defaults
  int i;
  for(i=0;i<lenq;i++){
    kK(pivots)[i] = ki(ni);
  }
  // Set First Pivot to Initial Pivot if requested, else NaN
  if(firstp->i == 1){
    kK(pivots)[0] = ki(initial_pivot->i);
  }
  
  
  int trend = (int)initial_pivot->i;
  int last_pivot_t = 0;
  double last_pivot_x = kF(x)[0];
  
  double x2;
  double r;

  
  for(i=1;i<lenq;i++){
    
    x2 = kF(x)[i];
    r = x2 / last_pivot_x;
    
    if(trend == -1){	// Uptrend
      if(r >= upthresh){
        kK(pivots)[last_pivot_t]->i = (int)trend;
        trend = 1;
        last_pivot_x = x2;
        last_pivot_t = i;
      }else if(x2 < last_pivot_x){
        last_pivot_x = x2;
        last_pivot_t = i;
      }
    } else {		// Downtrend
      if(r <= downthresh){
        kK(pivots)[last_pivot_t]->i = (int)trend;
        trend = -1;
        last_pivot_x = x2;
        last_pivot_t = i;
      }else if(x2 > last_pivot_x){
        last_pivot_x = x2;
        last_pivot_t = i;
      }
    }
    
  }
  
  
  if(last_pivot_t == t_n-1){
    kK(pivots)[last_pivot_t]->i = (int)trend;
  }else if(kK(pivots)[t_n-1] == 0){
    kK(pivots)[t_n-1]->i = (int)-trend;
  }
  
  return pivots;

}






K max_drawdown(K x)
{

  int typeq = x->t;
  int lenq = x->n;
  
  double mdd = 0;
  double dd;
  double peak = (double) kF(x)[0];
  
  int i;
  for(i=0;i<lenq;i++){
    if((double) kF(x)[i] > peak){
      peak = (double) kF(x)[i];
      dd = (peak - (double) kF(x)[i]) / peak;
      if(dd > mdd) {
        mdd = dd;
      }
    }
  }
  
  return kf(mdd);

}






K mafan(K a, K b, K c)
{

  int typeq = a->t;
  int lenq = a->n;
  
  if(lenq == 0){
    R krr("zlength");
  }          
  
  K fan = ktn(0, lenq);
  
  int i;
  for(i=0;i<lenq;i++){
    if((double) kF(a)[i] > (double) kF(b)[i] > (double) kF(c)[i]){
      kK(fan)[i] = ki(1);
    } else if ((double) kF(a)[i] < (double) kF(b)[i] < (double) kF(c)[i]){
      kK(fan)[i] = ki(-1);
    } else {
      kK(fan)[i] = ki(ni);
    }
  }
  
  return fan;

}




/*
OHLC Update
Input - Current Candle, bid, ask
Output - new Candle

*/
K ohlcupdate(K incdl, K price, K hl) {

  int typeq = incdl->t;
  int lenq = incdl->n;
  
  if(lenq == 0){
    R krr("zlength");
  }
  
  K out = ktn(9, lenq);
  
  int i = 0;
  for(i=0;i<lenq;i++){
    if(hl->i == 1 & (double)price->f > (double)kF(incdl)[i]){
      // High
      //kK(out)[i] = kf(price->f);
      kF(out)[i] = (double)price->f;
    } else if (hl->i == 0 & (double)price->f < (double)kF(incdl)[i]){
      // Low
      //kK(out)[i] = kf(price->f);
      kF(out)[i] = (double)price->f;
    } else if (hl->i == 2){
      //kK(out)[i] = kf(price->f);
      kF(out)[i] = (double)price->f;
    } else {
      //kK(out)[i] = kf(kF(incdl)[i]);
      kF(out)[i] = kF(incdl)[i];
    }
  }
                                          
  return out;
                                              

}
/*
K ohlcupdate(K incdl, K bid, K ask) {
  int typeq = incdl->t;
  int lenq = incdl->n;

  if(typeq != 98){
    R krr("ztype");
  }
  
  // Flip and get table data
  K flip=ktd(incdl);
  K colName = kK(flip->k)[0];
  K colData = kK(flip->k)[1];
  
  int nCols = colName->n;
  int nRows = kK(colData)[0]->n;
  
  int row;
  for(row=0;row<nRows;row++){
    K list = kK(colData)[0];
    K d = kK(list)[row];
  }
  
  K outCdl = incdl;
  
  
  return outCdl;
}
*/



/*
Candlestick pattern recognition
Return
  ni = Indecisive
  1 = Bull
  2 = Bear
*/
K cdl(K a, K b, K c, K d) {
  int typeq = a->t;
  int lenq = a->n;
  
  if(lenq == 0){
    R krr("zlength");
  }
  
  double aver_period = 50;	// Average Period for previous candles
  
  if(lenq < aver_period) {
    aver_period = lenq;
  }
  
  K outCdl = ktn(0, lenq);
  
  int i;
  int dir;
  int cdltype;
  double bodysize;
  double shade_low;
  double shade_high;
  double HL;
  double sum;


  // Calculate the average body size of previous candlesticks
  sum=0;
  int ii;
  for(ii=0; ii<aver_period; ii++){
    sum = sum + fabs(kF(a)[ii] - kF(d)[ii]);
  }
  sum = sum/aver_period;
  
                              
  for(i=0;i<lenq;i++){
    cdltype = 0;
    // Bull or Bear
    if(kF(a)[i] < kF(d)[i]){
      dir = 1;	
    } else {
      dir = 0;
    }
    
    // Get Candle Body Size
    bodysize = fabs(kF(a)[i] - kF(d)[i]);
    
    // Get Size of Shadows
    shade_low = kF(d)[i] - kF(c)[i];
    shade_high = kF(b)[i] - kF(a)[i];
    if(dir == 1) {
      shade_low = kF(a)[i] - kF(c)[i]; //res.open-res.low;
      shade_high = kF(b)[i] - kF(d)[i]; //res.high-res.close;
    }
    HL = kF(b)[i] - kF(c)[i]; //res.high-res.low;
    
    
    
    // Long Candlestick 
    if(bodysize>sum*1.3) {
      if(dir==1){
        cdltype=21;	//CAND_LONG;
      } else {
        cdltype=11;
      }
    }
    
    // Short Candlestick
    if(bodysize<sum*0.5) {
      if(dir==1){
        cdltype=22;	//CAND_SHORT;
      } else {
        cdltype=12;
      }
    }
    
    // Doji
    if(bodysize<HL*0.03) {
      if(dir==1){
        cdltype=23;	//CAND_DOJI;
      } else {
        cdltype=13;
      }
      
      if(shade_high*3 < shade_low){
        // Dragonfly Doji
        if(dir==1){
          cdltype=231;
        } else {
          cdltype=131;
        }
      }
      
      if(shade_low*3 < shade_high){
        // Graveyard Doji
        if(dir==1){
          cdltype=232;
        } else {
          cdltype=132;
        }
      }
      
    }
    
    // Maribozu
    if((shade_low<bodysize*0.01 || shade_high<bodysize*0.01) && bodysize>0) {
      if(cdltype==1) {	//Long
        if(dir==1){
          cdltype=24;		//CAND_MARIBOZU_LONG;
        } else {
          cdltype=14;
        }
      } else {
        if(dir==1){
          cdltype=25;		//CAND_MARIBOZU;
        } else {
          cdltype=15;
        }
      }
    }
    
    // Hammer
    if(shade_low>bodysize*2 && shade_high<bodysize*0.1){
      if(dir==1){
        cdltype=26;		//CAND_HAMMER;
      } else {
        cdltype=16;
      }
    }
    
    // Inverse Hammer
    if(shade_low<bodysize*0.1 && shade_high>bodysize*2){
      if(dir==1){
        cdltype=27;		//CAND_INVERT_HAMMER;
      } else {
        cdltype=17;
      }
    }
    
    // Spinning Top
    if((cdltype==12 || cdltype==22) && shade_low>bodysize && shade_high>bodysize){
      if(dir==1) {
        cdltype=28;
      } else {
        cdltype=18;	//CAND_SPIN_TOP;
      }
    }
    
    
    if(cdltype == 0) {
      kK(outCdl)[i] = ki(ni);
    } else {
      kK(outCdl)[i] = ki(cdltype);
    }  
    
  }
  
  return outCdl;
  
}




/*
Fractals

*/

K fractal(K high, K low) {
  
  int typeq = high->t;
  int lenq = high->n;
    
  if(lenq == 0){
    R krr("zlength");
  }

  K out = ktn(KI, lenq);

  int i = 0;
  for(i=4;i<lenq;i++){
    if(kF(high)[i-2] < kF(high)[i-1] & kF(high)[i-1] < kF(high)[i] & kF(high)[i] > kF(high)[i+1] & kF(high)[i+1] > kF(high)[i+2]){
      kI(out)[i] = 2;	// High Fractal with 5 bars
    } else if (kF(low)[i-2] > kF(low)[i-1] & kF(low)[i-1] > kF(low)[i] & kF(low)[i] < kF(low)[i+1] & kF(low)[i+1] < kF(low)[i+2]) {
      kI(out)[i] = -2;   // Low Fractal with 5 bars
    } else if (kF(high)[i-1] < kF(high)[i] & kF(high)[i] > kF(high)[i+1]){
      kI(out)[i] = 1;	// High with 3 bars
    } else if (kF(low)[i-1] > kF(low)[i] & kF(low)[i] < kF(low)[i+1]) {
      kI(out)[i] = -1;	//Low with 3 bars
    } else {
      kI(out)[i] = ni;
    }
  }
  
  return out;

}



