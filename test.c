extern unsigned long __VERIFIER_nondet_ulong();
extern int __VERIFIER_nondet_int();
extern unsigned char __VERIFIER_nondet_uchar();
extern unsigned short __VERIFIER_nondet_ushort();
extern unsigned int __VERIFIER_nondet_uint();
extern char __VERIFIER_nondet_char();
extern short __VERIFIER_nondet_short();
extern int __VERIFIER_nondet_int();
extern float __VERIFIER_nondet_float();
extern double __VERIFIER_nondet_double();
extern void __VERIFIER_assume(int);
extern void __VERIFIER_error();
unsigned short NEARLY_EQUAL(float a, float b)
{
  float diff = ((a - b) < 0) ? (- (a - b)) : (a - b);
  if (a == b)
  {
    return 1;
  }
  else
    if (((a == 0) || (b == 0)) || (diff < 1.17549435082228750796873653722224568e-38F))
  {
    return diff < (0.00001 * 1.17549435082228750796873653722224568e-38F);
  }
  else
  {
    return (diff / (((((a < 0) ? (- a) : (a)) + ((b < 0) ? (- b) : (b))) < 3.40282346638528859811704183484516925e+38F) ? (((a < 0) ? (- a) : (a)) + ((b < 0) ? (- b) : (b))) : (3.40282346638528859811704183484516925e+38F))) < 0.00001;
  }


}

typedef unsigned char U8;
typedef unsigned char FLAG;
typedef unsigned char MODE;
typedef unsigned short int U16;
typedef unsigned long int U32;
typedef unsigned long int TIME_MSEC;
typedef unsigned long int TIME_USEC;
typedef signed char S8;
typedef signed char SFLAG;
typedef signed char SMODE;
typedef signed short int S16;
typedef signed long int S32;
typedef float F32;
typedef float TIME_SEC;
typedef double F64;
typedef const struct 
{
  U8 size;
  volatile const F32 *array_ptr;
} TABLE_2D;
typedef const struct 
{
  U8 x_size;
  U8 y_size;
  TABLE_2D *x_normalizing_function;
  TABLE_2D *y_normalizing_function;
  volatile const F32 *array_ptr;
} TABLE_3D;
typedef enum 
{
  CLOSED_THROTTLE = - 1,
  PART_THROTTLE = 0,
  WIDE_OPEN_THROTTLE = 1
} APT_TYPE;
typedef struct _Atomic_FIFO_t
{
  void **ptr_array;
  U8 head;
  U8 tail;
  U8 queue_size;
  U8 queue_type_status;
  U32 queue_checksum;
  U32 array_checksum;
} Atomic_FIFO_t;
typedef union _Atomic_array_t
{
  Atomic_FIFO_t qptr;
  U32 qdata[4];
} Atomic_array_t;
U8 lib_utils_afifo_add(Atomic_FIFO_t *queue, const void *ptr);
void *lib_utils_afifo_remove(Atomic_FIFO_t *queue);
void lib_utils_afifo_lock(Atomic_FIFO_t *queue);
void lib_utils_afifo_unlock(Atomic_FIFO_t *queue);
void lib_utils_afifo_reset(Atomic_FIFO_t *queue);
U8 lib_utils_afifo_valid(const Atomic_FIFO_t *queue);
FLAG lib_utils_buffer_debounce(FLAG new, FLAG *PtrZ1, FLAG *PtrZ0);
FLAG lib_utils_buffer_bwindow(U8 *PtrBitBuf, FLAG new, U8 window);
F32 lib_utils_absolute_f32(F32 x);
F32 lib_utils_minimum_f32(F32 x1, F32 x2);
F32 lib_utils_maximum_f32(F32 x1, F32 x2);
F32 lib_utils_clipper_f32(F32 llim, F32 x, F32 ulim);
S32 lib_utils_absolute_s32(S32 x);
S32 lib_utils_minimum_s32(S32 x1, S32 x2);
S32 lib_utils_maximum_s32(S32 x1, S32 x2);
S32 lib_utils_clipper_s32(S32 llim, S32 x, S32 ulim);
U32 lib_utils_minimum_u32(U32 x1, U32 x2);
U32 lib_utils_maximum_u32(U32 x1, U32 x2);
U32 lib_utils_clipper_u32(U32 llim, U32 x, U32 ulim);
U32 lib_utils_clock_read(void);
U32 lib_utils_clock_delta_ms(U32 *ptr_clock);
F32 lib_utils_clock_delta_sec(U32 *ptr_clock);
typedef union _ONION32
{
  F32 F;
  U32 U;
  S32 S;
} ONION32;
typedef union _PTR_ONION32
{
  volatile const void *p_VOID;
  const F32 *p_FLOAT;
  const U32 *p_ULONG;
  const S32 *p_SLONG;
} PTR_ONION32;
typedef union _PTR_LOOKUP_DATA
{
  volatile const void *p_VOID;
  const F32 *p_FLOAT;
  const U16 *p_UINT;
  const U8 *p_UCHAR;
  const S16 *p_SINT;
  const S8 *p_SCHAR;
} PTR_LOOKUP_DATA;
U32 lib_utils_checktype_f32(volatile const F32 *PtrF32);
U32 lib_utils_decoded_f32(volatile const F32 *PtrF32);
U32 lib_utils_truncate_f32(F32 *PtrF32, U16 Trunc2Exp);
S32 lib_utils_binpoint_f32_s32(F32 DataF32, S8 BinPt);
F32 lib_utils_binpoint_s32_f32(S32 DataS32, S8 BinPt);
F32 lib_utils_linear_lpdd(F32 Z_Nu1, F32 fconst, F32 *pZ_Nu0, F32 Z_Den);
F32 lib_utils_linear_rlvt(F32 Z_den, F32 Z_num, F32 dt, F32 tc);
F32 lib_utils_linear_fcrs(F32 dt, F32 tc);
F32 lib_utils_linear_fcru(U32 dtms, F32 tc);
F32 lib_utils_linear_lpff(F32 Z_den, F32 Z_num, F32 fconst);
F32 lib_utils_linear_lpft(F32 Z_den, F32 Z_num, F32 dt, F32 tau);
F32 lib_utils_linear_lpfw(F32 Z_den, F32 Z_num, F32 dt, F32 omega);
F32 lib_utils_linear_hpft(F32 *Z_den, F32 Z_num, F32 dt, F32 tau);
F32 lib_utils_linear_fces(F32 dt, F32 tau);
F32 lib_utils_bilinear_lpt(F32 p_ZData[4], F32 Z_N2, F32 dt, F32 tau);
F32 lib_utils_bilinear_lpw(F32 p_ZData[4], F32 Z_N2, F32 dt, F32 omega);
F32 lib_utils_bilinear_lpwz(F32 p_ZData[4], F32 Z_N2, F32 dt, F32 omega, F32 zeta);
F32 lib_utils_bilinear_bpwz(F32 p_ZData[4], F32 Z_N2, F32 dt, F32 omega, F32 zeta);
F32 lib_utils_bilinear_hpwz(F32 p_ZData[4], F32 Z_N2, F32 dt, F32 omega, F32 zeta);
F32 lib_utils_bilinear_free(F32 p_ZData[4], volatile const F32 p_COEFS[6], F32 Z_N2);
F32 lib_utils_hysteresis_coulomb(F32 p_ZData[2], F32 Z_num, F32 hys);
F32 lib_utils_hysteresis_quantized(F32 Z_den, F32 Z_num, F32 hys);
F32 lib_utils_rate_clock(F32 Z_den, F32 Z_num, F32 dxdt_down, F32 dxdt_up, U32 *clock);
F32 lib_utils_rate_time(F32 Z_den, F32 Z_num, F32 dt, F32 dxdt_down, F32 dxdt_up);
typedef const struct _SMALL_AXIS
{
  U16 xLength;
  U16 xType;
  U16 xCoef_B;
  U16 xCoef_F;
  const volatile void *p_xData;
} SMALL_AXIS;
typedef const struct _SMALL_CURVE
{
  U16 yLength;
  U16 yType;
  U16 yCoef_B;
  U16 yCoef_F;
  SMALL_AXIS *p_xNormal;
  const volatile void *p_yData;
} SMALL_CURVE;
typedef const struct _SMALL_MAP
{
  U8 xLength;
  U8 yLength;
  U16 zType;
  U16 zCoef_B;
  U16 zCoef_F;
  SMALL_AXIS *p_xNormal;
  SMALL_AXIS *p_yNormal;
  const volatile void *p_zData;
} SMALL_MAP;
typedef const struct _TABLE_AXIS
{
  U8 xLength;
  U8 xMethod;
  U8 xType;
  U8 _space0_;
  const volatile void *p_xData;
  F32 xCoefs[4];
} TABLE_AXIS;
typedef const struct _TABLE_CURVE
{
  U8 yLength;
  U8 yMethod;
  U8 yType;
  U8 xTableKind;
  const void *p_xNormal;
  const volatile void *p_yData;
  F32 yCoefs[4];
} TABLE_CURVE;
typedef const struct _TABLE_MAP
{
  U8 xLength;
  U8 yLength;
  U8 zMethod;
  U8 zType;
  U8 xTableKind;
  U8 yTableKind;
  U8 _space0_;
  U8 _space1_;
  const void *p_xNormal;
  const void *p_yNormal;
  const volatile void *p_zData;
  F32 zCoefs[4];
} TABLE_MAP;
typedef const struct _TABLE_VALBLK
{
  U8 vLength;
  U8 vMethod;
  U8 vType;
  U8 _space0_;
  const volatile void *p_vData;
  F32 vCoefs[4];
} TABLE_VALBLK;
typedef const struct _SMALL_VALBLK
{
  U16 vLength;
  U16 vType;
  U16 vCoef_B;
  U16 vCoef_F;
  const volatile void *p_vData;
} SMALL_VALBLK;
F32 lib_utils_ramp_0to1(F32 x_val, F32 x_at_0, F32 x_at_1);
F32 lib_utils_ramp_1to0(F32 x_val, F32 x_at_1, F32 x_at_0);
U32 lib_utils_float_to_normal(F32 f_input);
F32 lib_utils_normal_to_float(U32 n_input);
F32 lib_utils_curve_hive_lg(TABLE_CURVE *PtrCurve, F32 X_in);
F32 lib_utils_map_hive_lg(TABLE_MAP *PtrMap, F32 X_in, F32 Y_in);
F32 lib_utils_curve_hive_sm(SMALL_CURVE *PtrCurve, F32 X_in);
F32 lib_utils_map_hive_sm(SMALL_MAP *PtrMap, F32 X_in, F32 Y_in);
F32 lib_utils_map_hive_3d(TABLE_3D *Ptr3D, F32 X_in, F32 Y_in);
F32 lib_utils_axis_frac_large(TABLE_AXIS *PtrAxis, F32 X_in);
U32 lib_utils_axis_index_large(TABLE_AXIS *PtrAxis, F32 X_in);
F32 lib_utils_valblk_pick_large(TABLE_VALBLK *PtrValBlk, U32 I_pick);
U32 lib_utils_axis_large(TABLE_AXIS *PtrAxis, F32 X_in);
F32 lib_utils_curve_large(TABLE_CURVE *PtrCurve, U32 X_nrm);
F32 lib_utils_map_large(TABLE_MAP *PtrMap, U32 X_nrm, U32 Y_nrm);
F32 lib_utils_axis_frac_small(SMALL_AXIS *PtrAxis, F32 X_in);
U32 lib_utils_axis_index_small(SMALL_AXIS *PtrAxis, F32 X_in);
F32 lib_utils_valblk_pick_small(SMALL_VALBLK *PtrValBlk, U32 I_pick);
U32 lib_utils_axis_small(SMALL_AXIS *PtrAxis, F32 X_in);
F32 lib_utils_curve_small(SMALL_CURVE *PtrCurve, U32 X_nrm);
F32 lib_utils_map_small(SMALL_MAP *PtrMap, U32 X_nrm, U32 Y_nrm);
F32 lib_utils_axis_2d(TABLE_2D *Ptr2D, F32 X_in);
F32 lib_utils_map_3d(TABLE_3D *Ptr3D, F32 X_nrm, F32 Y_nrm);
F32 lib_utils_math_log_f32(F32 x, U32 input_type);
F32 lib_utils_math_exp_f32(F32 power, U32 input_type);
F32 lib_utils_math_sin_f32(F32 angle, U32 input_type);
F32 lib_utils_math_arcsin_f32(F32 ratio, U32 input_type);
F32 lib_utils_math_tan_f32(F32 angle);
F32 lib_utils_math_arctan2_f32(F32 opposite, F32 base);
F32 lib_utils_math_arctan1_f32(F32 ratio);
F32 lib_utils_math_pow_f32(F32 x, F32 power);
F32 lib_utils_math_sqrt_f32(F32 x);
typedef union 
{
  S32 osc_data_signed;
  U32 osc_data_unsigned;
} osc_data;
typedef struct 
{
  osc_data osc_value;
  U8 osc_ena;
  U8 subst_req;
} OSC_IOCTL_CHANNEL;
typedef struct 
{
  U16 channel_identifier;
  U8 bit_position;
  U8 data_type;
  S8 bin_pt;
  U8 sign;
  OSC_IOCTL_CHANNEL *ram;
  U8 size;
} OSC_IOCTL_CHANNEL_ROM;
typedef union 
{
  FLAG *flag_val;
  U8 *u8_val;
  U16 *u16_val;
  U32 *u32_val;
  S8 *s8_val;
  S16 *s16_val;
  S32 *s32_val;
  F32 *f32_val;
  FLAG *bit_val;
} osc_subs_addr;
U32 CPU_DisableInts(void);
U32 CPU_EnableInts(void);
U32 CPU_RestoreInts(U32 prevstate);
void CPU_PutEIPR(U32 sfr_addr, U32 value);
typedef struct TBu64_s
{
  U32 low;
  U32 high;
} TBu64_t;
U32 PLL_SysClkMHz(void);
void TB_ReadTB(TBu64_t *pTBu64);
U32 TB_ReadTBL(void);
U32 TB_ReadTimeLow(void);
U32 TB_ReadTimeUsecsU64(TBu64_t *pUsecsU64);
void WDT_DisableWdog(void);
void WDT_ClrEndinit(void);
void WDT_SetEndinit(void);
U8 ShutupCY320(void);
F32 lib_utils_absolute_f32(F32 x)
{
  return (x < 0.0F) ? (- x) : (x);
}

F32 lib_utils_minimum_f32(F32 x1, F32 x2)
{
  return (x1 < x2) ? (x1) : (x2);
}

F32 lib_utils_maximum_f32(F32 x1, F32 x2)
{
  return (x1 > x2) ? (x1) : (x2);
}

F32 lib_utils_clipper_f32(F32 llim, F32 x, F32 ulim)
{
  return (x > llim) ? ((x < ulim) ? (x) : (ulim)) : (llim);
}

static U32 lib_utils_axis_large_api(TABLE_AXIS *PtrAxis, F32 X_In);
static U32 lib_utils_axis_small_api(SMALL_AXIS *PtrAxis, F32 X_In);
static U32 lib_utils_axis_engine(PTR_LOOKUP_DATA PtrData, U32 CellCount, U32 DataType, F32 X_Fit);
U32 lib_utils_axis_large(TABLE_AXIS *PtrAxis, F32 X_In)
{
  return lib_utils_axis_large_api(PtrAxis, X_In);
}

U32 lib_utils_axis_small(SMALL_AXIS *PtrAxis, F32 X_In)
{
  return lib_utils_axis_small_api(PtrAxis, X_In);
}

U32 lib_utils_axis_index_large(TABLE_AXIS *PtrAxis, F32 X_in)
{
  auto U32 u_output;
  auto U32 n_output;
  n_output = lib_utils_axis_large_api(PtrAxis, X_in);
  u_output = n_output >> 24U;
  if ((n_output & 0x00FFFFFFUL) >= 0x00800000UL)
  {
    u_output = u_output + 1U;
  }
  else
  {
  }

  return u_output;
}

U32 lib_utils_axis_index_small(SMALL_AXIS *PtrAxis, F32 X_in)
{
  auto U32 u_output;
  auto U32 n_output;
  n_output = lib_utils_axis_small_api(PtrAxis, X_in);
  u_output = n_output >> 24U;
  if ((n_output & 0x00FFFFFFUL) >= 0x00800000UL)
  {
    u_output = u_output + 1U;
  }
  else
  {
  }

  return u_output;
}

F32 lib_utils_axis_frac_large(TABLE_AXIS *PtrAxis, F32 X_in)
{
  auto U32 n_output;
  auto F32 f_output;
  n_output = lib_utils_axis_large_api(PtrAxis, X_in);
  f_output = (F32) n_output;
  f_output = f_output / 16777216.0F;
  return f_output;
}

F32 lib_utils_axis_frac_small(SMALL_AXIS *PtrAxis, F32 X_in)
{
  auto U32 n_output;
  auto F32 f_output;
  n_output = lib_utils_axis_small_api(PtrAxis, X_in);
  f_output = (F32) n_output;
  f_output = f_output / 16777216.0F;
  return f_output;
}

static U32 lib_utils_axis_large_api(TABLE_AXIS *PtrAxis, F32 X_In)
{
  auto ONION32 onion;
  auto PTR_LOOKUP_DATA PtrData;
  auto U32 CellCount;
  auto U32 DataType;
  auto F32 X_Fit;
  auto F32 X_num;
  auto F32 X_den;
  if (PtrAxis == 0)
  {
    return 0U;
  }
  else
  {
  }

  switch (PtrAxis->xMethod)
  {
    case 4U:

    case 1U:
      X_Fit = X_In;
      break;

    case 2U:
      X_Fit = (X_In * PtrAxis->xCoefs[0]) + PtrAxis->xCoefs[1];
      break;

    case 3U:
      X_num = (X_In * PtrAxis->xCoefs[0]) + PtrAxis->xCoefs[1];
      X_den = (X_In * PtrAxis->xCoefs[2]) + PtrAxis->xCoefs[3];
      onion.F = X_den;
      if ((onion.U & 0x7F800000UL) == 0UL)
    {
      X_Fit = 0.0F;
    }
    else
    {
      X_Fit = X_num / X_den;
    }

      break;

    default:
      return 0U;

  }

  DataType = PtrAxis->xType;
  CellCount = PtrAxis->xLength;
  PtrData.p_VOID = PtrAxis->p_xData;
  return lib_utils_axis_engine(PtrData, CellCount, DataType, X_Fit);
}

static U32 lib_utils_axis_small_api(SMALL_AXIS *PtrAxis, F32 X_In)
{
  auto PTR_LOOKUP_DATA PtrData;
  auto U32 CellCount;
  auto U32 DataType;
  auto U16 Coef_B;
  auto U16 Coef_F;
  auto F32 X_Fit;
  if (PtrAxis == 0)
  {
    return 0U;
  }
  else
  {
  }

  Coef_B = PtrAxis->xCoef_B;
  Coef_F = PtrAxis->xCoef_F;
  if ((Coef_B == 0U) || (Coef_F == 0U))
  {
    X_Fit = X_In;
  }
  else
    if (Coef_B == Coef_F)
  {
    X_Fit = X_In;
  }
  else
    if (Coef_B == 1U)
  {
    X_Fit = X_In / ((F32) Coef_F);
  }
  else
    if (Coef_F == 1U)
  {
    X_Fit = X_In * ((F32) Coef_B);
  }
  else
  {
    X_Fit = (X_In * ((F32) Coef_B)) / ((F32) Coef_F);
  }




  DataType = PtrAxis->xType;
  CellCount = PtrAxis->xLength;
  PtrData.p_VOID = PtrAxis->p_xData;
  return lib_utils_axis_engine(PtrData, CellCount, DataType, X_Fit);
}

static U32 lib_utils_axis_engine(PTR_LOOKUP_DATA PtrData, U32 CellCount, U32 DataType, F32 X_Fit)
{
  auto ONION32 onion;
  auto const U8 *XArrayA;
  auto const U16 *XArrayB;
  auto const S8 *XArrayC;
  auto const S16 *XArrayD;
  auto const F32 *XArrayE;
  auto F32 DeltaF1;
  auto F32 DeltaF2;
  auto U32 DeltaX1;
  auto U32 DeltaX2;
  auto U32 X_uint;
  auto U32 X_seeku;
  auto U32 X_uinta;
  auto U32 X_uintb;
  auto U32 index;
  auto U32 split;
  auto U32 n_output;
  auto S32 X_seeki;
  if (CellCount > 255U)
  {
    return 0U;
  }

  onion.F = X_Fit;
  index = CellCount - 1U;
  if ((onion.U & 0x7F800000UL) == 0x7F800000UL)
  {
    if (((onion.U & 0x007FFFFFUL) == 0UL) && ((onion.U & 0x80000000UL) == 0UL))
    {
      n_output = index << 24U;
    }
    else
    {
      n_output = 0U;
    }

  }
  else
  {
    switch (DataType)
    {
      case 0x21U:
        X_Fit = (0.0F > ((((F32) 0xFFU) < X_Fit) ? ((F32) 0xFFU) : (X_Fit))) ? (0.0F) : ((((F32) 0xFFU) < X_Fit) ? ((F32) 0xFFU) : (X_Fit));
        X_uint = (U32) ((X_Fit * 16777216.0F) + 0.5F);
        X_seeku = X_uint >> 24U;
        XArrayA = PtrData.p_UCHAR;
        if (X_seeku <= XArrayA[0])
      {
        n_output = 0U;
      }
      else
        if (X_seeku >= XArrayA[index])
      {
        n_output = index << 24U;
      }
      else
      {
        if (index >= 7U)
        {
          split = index >> 1U;
          if (X_seeku < XArrayA[split])
          {
            index = split;
          }
          else
          {
          }

        }
        else
        {
        }

        for (int i = 254; i >= 0; i--)
        {
          if ((i <= index) && (! (X_seeku < XArrayA[i])))
          {
            index = i;
            break;
          }

        }

        DeltaX1 = X_uint - (((U32) XArrayA[index]) << 24U);
        DeltaX2 = XArrayA[index + 1U] - XArrayA[index];
        DeltaX2 = (DeltaX1 + (DeltaX2 >> 1U)) / DeltaX2;
        n_output = (index << 24U) + DeltaX2;
      }


        break;

      case 0x22U:
        X_Fit = (0.0F > ((((F32) 0xFFFFU) < X_Fit) ? ((F32) 0xFFFFU) : (X_Fit))) ? (0.0F) : ((((F32) 0xFFFFU) < X_Fit) ? ((F32) 0xFFFFU) : (X_Fit));
        X_uint = (U32) ((X_Fit * 65536.0F) + 0.5F);
        X_seeku = X_uint >> 16U;
        XArrayB = PtrData.p_UINT;
        if (X_seeku <= XArrayB[0])
      {
        n_output = 0U;
      }
      else
        if (X_seeku >= XArrayB[index])
      {
        n_output = index << 24U;
      }
      else
      {
        if (index >= 7U)
        {
          split = index >> 1U;
          if (X_seeku < XArrayB[split])
          {
            index = split;
          }
          else
          {
          }

        }
        else
        {
        }

        for (int i = 254; i >= 0; i--)
        {
          if ((i <= index) && (! (X_seeku < XArrayB[i])))
          {
            index = i;
            break;
          }

        }

        DeltaX1 = X_uint - (((U32) XArrayB[index]) << 16U);
        DeltaX2 = XArrayB[index + 1U] - XArrayB[index];
        DeltaX2 = ((DeltaX1 + (DeltaX2 >> 1U)) / DeltaX2) << 8U;
        n_output = (index << 24U) + DeltaX2;
      }


        break;

      case 0x31U:
        X_Fit = (((F32) ((- 127) - 1)) > ((((F32) 127) < X_Fit) ? ((F32) 127) : (X_Fit))) ? ((F32) ((- 127) - 1)) : ((((F32) 127) < X_Fit) ? ((F32) 127) : (X_Fit));
        X_uint = (U32) (((X_Fit + 128.0F) * 16777216.0F) + 0.5F);
        X_seeki = ((S32) (X_uint / 16777216UL)) - 128L;
        XArrayC = PtrData.p_SCHAR;
        if (X_seeki <= XArrayC[0])
      {
        n_output = 0U;
      }
      else
        if (X_seeki >= XArrayC[index])
      {
        n_output = index << 24U;
      }
      else
      {
        if (index >= 7U)
        {
          split = index >> 1U;
          if (X_seeki < XArrayC[split])
          {
            index = split;
          }
          else
          {
          }

        }
        else
        {
        }

        for (int i = 254; i >= 0; i--)
        {
          if ((i <= index) && (! (X_seeki < XArrayC[i])))
          {
            index = i;
            break;
          }

        }

        X_uinta = (U32) (128L + ((S32) XArrayC[index]));
        X_uintb = (U32) (128L + ((S32) XArrayC[index + 1U]));
        DeltaX1 = X_uint - (X_uinta << 24U);
        DeltaX2 = X_uintb - X_uinta;
        DeltaX2 = (DeltaX1 + (DeltaX2 >> 1U)) / DeltaX2;
        n_output = (index << 24U) + DeltaX2;
      }


        break;

      case 0x32U:
        X_Fit = (((F32) ((- 32767) - 1)) > ((((F32) 32767) < X_Fit) ? ((F32) 32767) : (X_Fit))) ? ((F32) ((- 32767) - 1)) : ((((F32) 32767) < X_Fit) ? ((F32) 32767) : (X_Fit));
        X_uint = (U32) (((X_Fit + 32768.0F) * 65536.0F) + 0.5F);
        X_seeki = ((S32) (X_uint / 65536UL)) - 32768L;
        XArrayD = PtrData.p_SINT;
        if (X_seeki <= XArrayD[0])
      {
        n_output = 0U;
      }
      else
        if (X_seeki >= XArrayD[index])
      {
        n_output = index << 24U;
      }
      else
      {
        if (index >= 7U)
        {
          split = index >> 1U;
          if (X_seeki < XArrayD[split])
          {
            index = split;
          }
          else
          {
          }

        }
        else
        {
        }

        for (int i = 254; i >= 0; i--)
        {
          if ((i <= index) && (! (X_seeki < XArrayD[i])))
          {
            index = i;
            break;
          }

        }

        X_uinta = (U32) (32768L + ((S32) XArrayD[index]));
        X_uintb = (U32) (32768L + ((S32) XArrayD[index + 1U]));
        DeltaX1 = X_uint - (X_uinta << 16U);
        DeltaX2 = X_uintb - X_uinta;
        DeltaX2 = ((DeltaX1 + (DeltaX2 >> 1U)) / DeltaX2) << 8U;
        n_output = (index << 24U) + DeltaX2;
      }


        break;

      case 0x11U:
        XArrayE = PtrData.p_FLOAT;
        if (X_Fit <= XArrayE[0])
      {
        n_output = 0U;
      }
      else
        if (X_Fit >= XArrayE[index])
      {
        n_output = index << 24U;
      }
      else
      {
        if (index >= 7U)
        {
          split = index >> 1U;
          if (X_Fit < XArrayE[split])
          {
            index = split;
          }
          else
          {
          }

        }
        else
        {
        }

        for (int i = 254; i >= 0; i--)
        {
          if ((i <= index) && (! (X_Fit < XArrayE[i])))
          {
            index = i;
            break;
          }

        }

        DeltaF1 = X_Fit - XArrayE[index];
        DeltaF2 = XArrayE[index + 1U] - XArrayE[index];
        DeltaF2 = (DeltaF1 / DeltaF2) * 16777216.0F;
        n_output = (index << 24U) + ((U32) DeltaF2);
      }


        break;

      default:
        return 0U;

    }

  }

  return n_output;
}

static F32 lib_utils_curve_small_api(SMALL_CURVE *PtrCurve, U32 X_Nrm);
static F32 lib_utils_curve_large_api(TABLE_CURVE *PtrCurve, U32 X_Nrm);
static F32 lib_utils_curve_engine(PTR_LOOKUP_DATA PtrData, U32 Size_Y, U32 Type, U32 X_Nrm);
F32 lib_utils_valblk_pick_large(TABLE_VALBLK *PtrValBlk, U32 I_pick)
{
  auto U32 i;
  auto union _LOCAL_LARGE_UNION
  {
    const void *pLV;
    TABLE_CURVE *pLC;
  } pLU;
  auto struct _LOCAL_LARGE_CURVE
  {
    U8 yLength;
    U8 yMethod;
    U8 yType;
    U8 xTableKind;
    const void *p_xNormal;
    const volatile void *p_yData;
    F32 yCoefs[4];
  } lg_curve;
  lg_curve.yLength = PtrValBlk->vLength;
  lg_curve.yMethod = PtrValBlk->vMethod;
  lg_curve.yType = PtrValBlk->vType;
  lg_curve.xTableKind = 0U;
  lg_curve.p_xNormal = 0;
  lg_curve.p_yData = PtrValBlk->p_vData;
  pLU.pLV = & lg_curve;
  for (i = 0; i < 4; i++)
  {
    lg_curve.yCoefs[i] = PtrValBlk->vCoefs[i];
  }

  if (I_pick > 255U)
  {
    I_pick = 0xFF000000UL;
  }
  else
  {
    I_pick = I_pick << 24U;
  }

  return lib_utils_curve_large_api(pLU.pLC, I_pick);
}

F32 lib_utils_valblk_pick_small(SMALL_VALBLK *PtrValBlk, U32 I_pick)
{
  auto union _LOCAL_SMALL_UNION
  {
    const void *pSV;
    SMALL_CURVE *pSC;
  } pSU;
  auto struct _LOCAL_SMALL_CURVE
  {
    U16 yLength;
    U16 yType;
    U16 yCoef_B;
    U16 yCoef_F;
    SMALL_AXIS *p_xNormal;
    const volatile void *p_yData;
  } sm_curve;
  sm_curve.yLength = PtrValBlk->vLength;
  sm_curve.yType = PtrValBlk->vType;
  sm_curve.yCoef_B = PtrValBlk->vCoef_B;
  sm_curve.yCoef_F = PtrValBlk->vCoef_F;
  sm_curve.p_xNormal = 0;
  sm_curve.p_yData = PtrValBlk->p_vData;
  pSU.pSV = & sm_curve;
  if (I_pick > 255U)
  {
    I_pick = 0xFF000000UL;
  }
  else
  {
    I_pick = I_pick << 24U;
  }

  return lib_utils_curve_small_api(pSU.pSC, I_pick);
}

F32 lib_utils_curve_large(TABLE_CURVE *PtrCurve, U32 X_Nrm)
{
  return lib_utils_curve_large_api(PtrCurve, X_Nrm);
}

F32 lib_utils_curve_small(SMALL_CURVE *PtrCurve, U32 X_Nrm)
{
  return lib_utils_curve_small_api(PtrCurve, X_Nrm);
}

static F32 lib_utils_curve_large_api(TABLE_CURVE *PtrCurve, U32 X_Nrm)
{
  auto ONION32 onion;
  auto PTR_LOOKUP_DATA PtrData;
  auto U32 Size_Y;
  auto U32 Type;
  auto F32 f_output;
  auto F32 f_outden;
  auto F32 f_outnum;
  if (PtrCurve == 0)
  {
    return 0.0F;
  }
  else
  {
  }

  Type = PtrCurve->yType;
  Size_Y = PtrCurve->yLength;
  PtrData.p_VOID = PtrCurve->p_yData;
  f_output = lib_utils_curve_engine(PtrData, Size_Y, Type, X_Nrm);
  switch (PtrCurve->yMethod)
  {
    case 4U:

    case 1U:
      break;

    case 2U:
      f_outden = PtrCurve->yCoefs[0];
      onion.F = f_outden;
      if ((onion.U & 0x7F800000UL) == 0UL)
    {
      return 0.0F;
    }
    else
    {
      f_output = (f_output - PtrCurve->yCoefs[1]) / f_outden;
    }

      break;

    case 3U:
      f_outden = ((- f_output) * PtrCurve->yCoefs[2]) + PtrCurve->yCoefs[0];
      onion.F = f_outden;
      if ((onion.U & 0x7F800000UL) == 0UL)
    {
      return 0.0F;
    }
    else
    {
      f_outnum = (f_output * PtrCurve->yCoefs[3]) - PtrCurve->yCoefs[1];
      f_output = f_outnum / f_outden;
    }

      break;

    default:
      return 0.0F;

  }

  return f_output;
}

static F32 lib_utils_curve_small_api(SMALL_CURVE *PtrCurve, U32 X_Nrm)
{
  auto PTR_LOOKUP_DATA PtrData;
  auto U16 Coef_B;
  auto U16 Coef_F;
  auto U32 Size_Y;
  auto U32 Type;
  auto F32 f_output;
  auto F32 f_inter;
  if (PtrCurve == 0)
  {
    return 0.0F;
  }
  else
  {
  }

  Type = PtrCurve->yType;
  Size_Y = PtrCurve->yLength;
  PtrData.p_VOID = PtrCurve->p_yData;
  f_inter = lib_utils_curve_engine(PtrData, Size_Y, Type, X_Nrm);
  Coef_B = PtrCurve->yCoef_B;
  Coef_F = PtrCurve->yCoef_F;
  if ((Coef_B == 0U) || (Coef_F == 0U))
  {
    f_output = f_inter;
  }
  else
    if (Coef_B == Coef_F)
  {
    f_output = f_inter;
  }
  else
    if (Coef_B == 1U)
  {
    f_output = f_inter * ((F32) Coef_F);
  }
  else
    if (Coef_F == 1U)
  {
    f_output = f_inter / ((F32) Coef_B);
  }
  else
  {
    f_output = (f_inter * ((F32) Coef_F)) / ((F32) Coef_B);
  }




  return f_output;
}

static F32 lib_utils_curve_engine(PTR_LOOKUP_DATA PtrData, U32 Size_Y, U32 Type, U32 X_Nrm)
{
  auto const U8 *YArrayA;
  auto const U16 *YArrayB;
  auto const S8 *YArrayC;
  auto const S16 *YArrayD;
  auto const F32 *YArrayE;
  auto U32 index1;
  auto U32 index2;
  auto U32 weight1;
  auto U32 weight2;
  auto F32 wfloat1;
  auto F32 wfloat2;
  auto U32 u_output;
  auto S32 s_output;
  auto F32 f_output;
  if (Size_Y > 255U)
  {
    return 0.0F;
  }

  index1 = X_Nrm >> 24U;
  if (index1 >= (Size_Y - 1UL))
  {
    index1 = Size_Y - 1UL;
    index2 = index1;
    weight2 = 0UL;
    weight1 = 16777216UL;
  }
  else
  {
    index2 = index1 + 1U;
    weight2 = X_Nrm & 0x00FFFFFFUL;
    weight1 = 16777216UL - weight2;
  }

  switch (Type)
  {
    case 0x21U:
      YArrayA = PtrData.p_UCHAR;
      u_output = YArrayA[index1] * weight1;
      u_output = u_output + (YArrayA[index2] * weight2);
      f_output = ((F32) u_output) / 16777216.0F;
      break;

    case 0x31U:
      YArrayC = PtrData.p_SCHAR;
      s_output = YArrayC[index1] * ((S32) weight1);
      s_output = s_output + (YArrayC[index2] * ((S32) weight2));
      f_output = ((F32) s_output) / 16777216.0F;
      break;

    case 0x22U:
      if (weight2 > weight1)
    {
      weight2 = (weight2 + 0x80UL) >> 8U;
      weight1 = 65536UL - weight2;
    }
    else
    {
      weight1 = (weight1 + 0x80UL) >> 8U;
      weight2 = 65536UL - weight1;
    }

      YArrayB = PtrData.p_UINT;
      u_output = YArrayB[index1] * weight1;
      u_output = u_output + (YArrayB[index2] * weight2);
      f_output = ((F32) u_output) / 65536.0F;
      break;

    case 0x32U:
      if (weight2 > weight1)
    {
      weight2 = (weight2 + 0x80UL) >> 8U;
      weight1 = 65536UL - weight2;
    }
    else
    {
      weight1 = (weight1 + 0x80UL) >> 8U;
      weight2 = 65536UL - weight1;
    }

      YArrayD = PtrData.p_SINT;
      s_output = YArrayD[index1] * ((S32) weight1);
      s_output = s_output + (YArrayD[index2] * ((S32) weight2));
      f_output = ((F32) s_output) / 65536.0F;
      break;

    case 0x11U:
      wfloat2 = ((F32) weight2) / 16777216.0F;
      wfloat1 = 1.0F - wfloat2;
      YArrayE = PtrData.p_FLOAT;
      f_output = YArrayE[index1] * wfloat1;
      f_output = f_output + (YArrayE[index2] * wfloat2);
      break;

    default:
      return 0.0F;

  }

  return f_output;
}

void VcVmcDsr(void);
void RESTART_VcVmcDsr(void);
typedef struct 
{
  U8 UnitDelay2;
} rtB_x3140_MTIDC_VcVmcDsr;
typedef struct 
{
  U8 UnitDelay1_DSTATE;
  U8 UnitDelay2_DSTATE;
  FLAG UnitDelay1_DSTATE_m;
  FLAG UnitDelay1_DSTATE_p;
  FLAG UnitDelay1_DSTATE_a;
  FLAG UnitDelay1_DSTATE_c;
} rtDW_x3140_MTIDC_VcVmcDsr;
typedef struct 
{
  U8 is_active_c3_VcVmcDsr_lib;
  U8 is_c3_VcVmcDsr_lib;
} rtDW_x3230_MTIDC_VcVmcDsr;
typedef struct 
{
  F32 t_Timer;
  U8 is_active_c5_VcVmcDsr_lib;
  U8 is_c5_VcVmcDsr_lib;
  U8 is_TRANSFER_dVcVmcDsr_D_DrvlnCm;
} rtDW_x4200_MTIDC_VcVmcDsr;
typedef struct 
{
  F32 Sum2;
  F32 Assignment1[10];
  F32 Assignment1_d[10];
  FLAG VariantMergeForOutportxVcVmcDsr;
  FLAG xVcVmcDsr_B_IdclInhbtAftrDrvlnC;
  rtB_x3140_MTIDC_VcVmcDsr x3140_MTIDC;
} rtB_VcVmcDsr_ODC_Present_VcVmcD;
typedef struct 
{
  F32 UnitDelay6_DSTATE;
  F32 UnitDelay6_DSTATE_n;
  F32 UnitDelay6_DSTATE_e;
  F32 UnitDelay6_DSTATE_e0[10];
  F32 UnitDelay2_DSTATE[10];
  F32 UnitDelay7_DSTATE;
  F32 UnitDelay3_DSTATE;
  F32 UnitDelay2_DSTATE_h;
  F32 UnitDelay_DSTATE;
  F32 UnitDelay_DSTATE_e;
  F32 t_Timer;
  U8 UnitDelay1_DSTATE;
  U8 UnitDelay_DSTATE_ew;
  FLAG UnitDelay5_DSTATE;
  FLAG UnitDelay1_DSTATE_n;
  FLAG UnitDelay1_DSTATE_ne;
  FLAG UnitDelay3_DSTATE_o;
  FLAG UnitDelay1_DSTATE_j;
  FLAG UnitDelay_DSTATE_o;
  FLAG UnitDelay1_DSTATE_d;
  FLAG UnitDelay1_DSTATE_f;
  FLAG UnitDelay1_DSTATE_fe;
  FLAG UnitDelay1_DSTATE_b;
  FLAG UnitDelay1_DSTATE_e;
  FLAG UnitDelay1_DSTATE_fx;
  FLAG UnitDelay1_DSTATE_bx;
  FLAG UnitDelay1_DSTATE_o;
  FLAG UnitDelay1_DSTATE_fz;
  FLAG UnitDelay1_DSTATE_bh;
  FLAG UnitDelay_DSTATE_o4;
  FLAG UnitDelay1_DSTATE_js;
  U8 is_active_c15_VcVmcDsr_lib;
  U8 is_c15_VcVmcDsr_lib;
  U8 is_NORMAL_OPERATION;
  U8 is_IN_IDC_SPEED_RANGE;
  U8 is_IDC_AVAILABILITY;
  U8 is_OUT_OF_IDC_SPEED_RANGE;
  U8 is_active_c10_VcVmcDsr_lib;
  U8 is_c10_VcVmcDsr_lib;
  rtDW_x4200_MTIDC_VcVmcDsr x4200_MTIDC;
  rtDW_x3230_MTIDC_VcVmcDsr x3230_MTIDC;
  rtDW_x3140_MTIDC_VcVmcDsr x3140_MTIDC;
} rtDW_VcVmcDsr_ODC_Present_VcVmc;
typedef struct 
{
  rtB_VcVmcDsr_ODC_Present_VcVmcD VcVmcDsr_ODC_Present;
} BlockIO_VcVmcDsr;
typedef struct 
{
  F32 pvc_fast_ts;
  rtDW_VcVmcDsr_ODC_Present_VcVmc VcVmcDsr_ODC_Present;
} D_Work_VcVmcDsr;
typedef struct 
{
  F32 pooled4[10];
} ConstParam_VcVmcDsr;
struct 
{
  struct 
  {
    float Sum2;
    float Assignment1[10];
    float Assignment1_d[10];
    unsigned char VariantMergeForOutportxVcVmcDsr;
    unsigned char xVcVmcDsr_B_IdclInhbtAftrDrvlnC;
    struct 
    {
      unsigned char UnitDelay2;
    } x3140_MTIDC;
  } VcVmcDsr_ODC_Present;
} VcVmcDsr_B;
struct 
{
  float pvc_fast_ts;
  struct 
  {
    float UnitDelay6_DSTATE;
    float UnitDelay6_DSTATE_n;
    float UnitDelay6_DSTATE_e;
    float UnitDelay6_DSTATE_e0[10];
    float UnitDelay2_DSTATE[10];
    float UnitDelay7_DSTATE;
    float UnitDelay3_DSTATE;
    float UnitDelay2_DSTATE_h;
    float UnitDelay_DSTATE;
    float UnitDelay_DSTATE_e;
    float t_Timer;
    unsigned char UnitDelay1_DSTATE;
    unsigned char UnitDelay_DSTATE_ew;
    unsigned char UnitDelay5_DSTATE;
    unsigned char UnitDelay1_DSTATE_n;
    unsigned char UnitDelay1_DSTATE_ne;
    unsigned char UnitDelay3_DSTATE_o;
    unsigned char UnitDelay1_DSTATE_j;
    unsigned char UnitDelay_DSTATE_o;
    unsigned char UnitDelay1_DSTATE_d;
    unsigned char UnitDelay1_DSTATE_f;
    unsigned char UnitDelay1_DSTATE_fe;
    unsigned char UnitDelay1_DSTATE_b;
    unsigned char UnitDelay1_DSTATE_e;
    unsigned char UnitDelay1_DSTATE_fx;
    unsigned char UnitDelay1_DSTATE_bx;
    unsigned char UnitDelay1_DSTATE_o;
    unsigned char UnitDelay1_DSTATE_fz;
    unsigned char UnitDelay1_DSTATE_bh;
    unsigned char UnitDelay_DSTATE_o4;
    unsigned char UnitDelay1_DSTATE_js;
    unsigned char is_active_c15_VcVmcDsr_lib;
    unsigned char is_c15_VcVmcDsr_lib;
    unsigned char is_NORMAL_OPERATION;
    unsigned char is_IN_IDC_SPEED_RANGE;
    unsigned char is_IDC_AVAILABILITY;
    unsigned char is_OUT_OF_IDC_SPEED_RANGE;
    unsigned char is_active_c10_VcVmcDsr_lib;
    unsigned char is_c10_VcVmcDsr_lib;
    struct 
    {
      float t_Timer;
      unsigned char is_active_c5_VcVmcDsr_lib;
      unsigned char is_c5_VcVmcDsr_lib;
      unsigned char is_TRANSFER_dVcVmcDsr_D_DrvlnCm;
    } x4200_MTIDC;
    struct 
    {
      unsigned char is_active_c3_VcVmcDsr_lib;
      unsigned char is_c3_VcVmcDsr_lib;
    } x3230_MTIDC;
    struct 
    {
      unsigned char UnitDelay1_DSTATE;
      unsigned char UnitDelay2_DSTATE;
      unsigned char UnitDelay1_DSTATE_m;
      unsigned char UnitDelay1_DSTATE_p;
      unsigned char UnitDelay1_DSTATE_a;
      unsigned char UnitDelay1_DSTATE_c;
    } x3140_MTIDC;
  } VcVmcDsr_ODC_Present;
} VcVmcDsr_DWork;
void VcVmcDsr_initialize(void);
void VcVmcDsr_step(void);
volatile const unsigned char cVcVmcDsr_B_CcIdcAllow;
volatile const unsigned char cVcVmcDsr_B_DsrStatsRst;
volatile const unsigned char cVcVmcDsr_B_IdcSelSwAsIdcTerm;
volatile const unsigned char cVcVmcDsr_B_IdcTermClPed;
volatile const unsigned char cVcVmcDsr_B_IdcTermGearChngdWoClPed;
volatile const unsigned char cVcVmcDsr_B_IdcTermGearDnshft;
volatile const unsigned char cVcVmcDsr_B_SpdIncRstIdcTerm;
volatile const unsigned char cVcVmcDsr_B_TermIdcInhClrPed;
volatile const unsigned char cVcVmcDsr_B_TermIdcInhClrPedCl;
volatile const unsigned char cVcVmcDsr_B_UseAccPed;
volatile const unsigned char cVcVmcDsr_D_GearNtrl;
volatile const unsigned char cVcVmcDsr_D_IdcIhbtClassAsgnmt[32];
volatile const float cVcVmcDsr_L_ClassIdcDist[9];
volatile const float cVcVmcDsr_L_MinIdcEvnt2Evnt;
volatile const float cVcVmcDsr_X_RdGradeIdcInhHyst;
volatile const float cVcVmcDsr_X_RdGradeNegIdcInhThres;
volatile const float cVcVmcDsr_X_RdGradePosIdcInhThres;
volatile const float cVcVmcDsr_Xd_AccPedIdcInh;
volatile const float cVcVmcDsr_Z_AccPedPrsdHyst;
volatile const float cVcVmcDsr_Z_AccPedPrsdThres;
volatile const float cVcVmcDsr_Z_FiltCnstIdcDistRt;
volatile const unsigned char cVcVmcDsr_Z_SpdIncCountLim;
volatile const float cVcVmcDsr_Z_SpdIncFltCnst;
volatile const float cVcVmcDsr_n_HystNiIdcTerm;
volatile const float cVcVmcDsr_t_AccPedGradTimeOut;
volatile const float cVcVmcDsr_t_ClassIdcDurtn[9];
volatile const float cVcVmcDsr_t_DsrTimerMaxVal;
volatile const float cVcVmcDsr_t_IdcDelay;
volatile const float cVcVmcDsr_t_TermIdcInhTout;
volatile const float cVcVmcDsr_ts;
volatile const float cVcVmcDsr_v_DsrMaxVehHyst;
volatile const float cVcVmcDsr_v_DsrMaxVehThres;
volatile const float cVcVmcDsr_v_DsrMinVehHyst;
volatile const float cVcVmcDsr_v_DsrMinVehThres;
volatile const float cVcVmcDsr_v_SpdIncMinVehTol;
volatile const unsigned char tVcVmcDsr_B_GearAsIdcTerm[12];
unsigned char TBD_DrvSituationDetection_IdcInhbt;
unsigned char TBD_DrvSituationDetection_NoIdcReq;
unsigned char TBD_Spare_IdcInhbt;
unsigned char TBD_Spare_NoIdcReq;
unsigned char XXXTrnGearState_No_Qf;
unsigned char XXX_D_Gear;
unsigned char sVcDeScl_D_CcStatus;
float sVcDeScl_v_CcSet;
unsigned char sVcDi_Qf_AccPed;
float sVcDi_X_AccPed;
float sVcDi_Xd_AccPed;
float sVcDtcAtf_Tq_Min;
float sVcEc_v_Veh;
unsigned char sVcIfBard_Qf_BpedStat;
unsigned char sVcIfRge_Qf_RdGrade;
float sVcIfRge_X_RdGrade;
unsigned char sVcTc_D_CpedState;
unsigned char sVcTc_D_CpedStateChk;
float sVcTc_n_GearBoxIn;
float sVcVmcAjc_Tq_Req;
unsigned char sVcVmcDsr_D_IdcState;
unsigned char sVcVmcPmm_D_DrvlnState;
unsigned char sVcVmcPmm_D_PtcIdcError;
unsigned char xVcVmcDsr_B_HystStateVehSpdMax;
unsigned char xVcVmcDsr_B_HystStateVehSpdMin;
unsigned char xVcVmcDsr_B_IdcInhbt;
unsigned char xVcVmcDsr_B_OutOfIdcVehSpdRng;
unsigned char yVcDi_B_IdcSelSw;
unsigned char yVcIfBard_B_BpedStat;
struct 
{
  unsigned char xLength;
  unsigned char xMethod;
  unsigned char xType;
  unsigned char _space0_;
  const volatile void *p_xData;
  float xCoefs[4];
} tVcVmcDsr_v_CcDeltaIdcTerm_x;
struct 
{
  unsigned char xLength;
  unsigned char xMethod;
  unsigned char xType;
  unsigned char _space0_;
  const volatile void *p_xData;
  float xCoefs[4];
} tVcVmcDsr_v_SpdIncIdcTerm_x;
FLAG etime_idc(F32 t0, F32 ti, F32 duration);
unsigned char TBD_DrvSituationDetection_IdcInhbt = 0;
unsigned char TBD_DrvSituationDetection_NoIdcReq = 0;
unsigned char TBD_Spare_IdcInhbt = 0;
unsigned char TBD_Spare_NoIdcReq = 0;
unsigned char XXXTrnGearState_No_Qf = (U8) 0U;
unsigned char XXX_D_Gear = (U8) 0U;
unsigned char sVcDeScl_D_CcStatus = (U8) 0U;
float sVcDeScl_v_CcSet = 0.0F;
unsigned char sVcDi_Qf_AccPed = (U8) 0U;
float sVcDi_X_AccPed = 0.0F;
float sVcDi_Xd_AccPed = 0.0F;
float sVcDtcAtf_Tq_Min = 0.0F;
float sVcEc_v_Veh = 0.0F;
unsigned char sVcIfBard_Qf_BpedStat = (U8) 0U;
unsigned char sVcIfRge_Qf_RdGrade = (U8) 0U;
float sVcIfRge_X_RdGrade = 0.0F;
unsigned char sVcTc_D_CpedState = (U8) 0U;
unsigned char sVcTc_D_CpedStateChk = (U8) 0U;
float sVcTc_n_GearBoxIn = 0.0F;
float sVcVmcAjc_Tq_Req = 0.0F;
unsigned char sVcVmcPmm_D_DrvlnState = (U8) 0U;
unsigned char sVcVmcPmm_D_PtcIdcError = (U8) 0U;
unsigned char yVcDi_B_IdcSelSw = 0;
unsigned char yVcIfBard_B_BpedStat = (U8) 0U;
volatile const float tVcVmcDsr_v_CcDeltaIdcTerm_x_array[8] = {0.0F, 5.0F, 10.0F, 20.0F, 30.0F, 60.0F, 100.0F, 200.0F};
struct 
{
  unsigned char xLength;
  unsigned char xMethod;
  unsigned char xType;
  unsigned char _space0_;
  const volatile void *p_xData;
  float xCoefs[4];
} tVcVmcDsr_v_CcDeltaIdcTerm_x = {8, 2U, 0x11U, 0, & tVcVmcDsr_v_CcDeltaIdcTerm_x_array[0], {1.0F, 0.0F, 0.0F, 1.0F}};
volatile const float tVcVmcDsr_v_SpdIncIdcTerm_x_array[6] = {0.0F, 5.0F, 10.0F, 50.0F, 100.0F, 320.0F};
struct 
{
  unsigned char xLength;
  unsigned char xMethod;
  unsigned char xType;
  unsigned char _space0_;
  const volatile void *p_xData;
  float xCoefs[4];
} tVcVmcDsr_v_SpdIncIdcTerm_x = {6, 2U, 0x11U, 0, & tVcVmcDsr_v_SpdIncIdcTerm_x_array[0], {1.0F, 0.0F, 0.0F, 1.0F}};
volatile const float tVcVmcDsr_v_CcDeltaIdcTerm_y_array[8] = {2.0F, 2.0F, 3.0F, 3.0F, 3.0F, 3.0F, 3.0F, 3.0F};
volatile const float tVcVmcDsr_v_SpdIncIdcTerm_y_array[6] = {2.0F, 2.0F, 3.0F, 3.0F, 5.0F, 5.0F};
volatile const float cVcVmcDsr_n_MaxNiIdcTerm_array[7] = {2500.0F, 2500.0F, 2500.0F, 2500.0F, 2500.0F, 2500.0F, 2500.0F};
volatile const float cVcVmcDsr_n_MinNiIdcTerm_array[7] = {0.0F, 900.0F, 900.0F, 900.0F, 900.0F, 900.0F, 900.0F};
volatile const float tVcVmcDsr_D_IdcValidGears_array[12] = {0.0F, 1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F};
struct 
{
  struct 
  {
    float Sum2;
    float Assignment1[10];
    float Assignment1_d[10];
    unsigned char VariantMergeForOutportxVcVmcDsr;
    unsigned char xVcVmcDsr_B_IdclInhbtAftrDrvlnC;
    struct 
    {
      unsigned char UnitDelay2;
    } x3140_MTIDC;
  } VcVmcDsr_ODC_Present;
} VcVmcDsr_B;
struct 
{
  float pvc_fast_ts;
  struct 
  {
    float UnitDelay6_DSTATE;
    float UnitDelay6_DSTATE_n;
    float UnitDelay6_DSTATE_e;
    float UnitDelay6_DSTATE_e0[10];
    float UnitDelay2_DSTATE[10];
    float UnitDelay7_DSTATE;
    float UnitDelay3_DSTATE;
    float UnitDelay2_DSTATE_h;
    float UnitDelay_DSTATE;
    float UnitDelay_DSTATE_e;
    float t_Timer;
    unsigned char UnitDelay1_DSTATE;
    unsigned char UnitDelay_DSTATE_ew;
    unsigned char UnitDelay5_DSTATE;
    unsigned char UnitDelay1_DSTATE_n;
    unsigned char UnitDelay1_DSTATE_ne;
    unsigned char UnitDelay3_DSTATE_o;
    unsigned char UnitDelay1_DSTATE_j;
    unsigned char UnitDelay_DSTATE_o;
    unsigned char UnitDelay1_DSTATE_d;
    unsigned char UnitDelay1_DSTATE_f;
    unsigned char UnitDelay1_DSTATE_fe;
    unsigned char UnitDelay1_DSTATE_b;
    unsigned char UnitDelay1_DSTATE_e;
    unsigned char UnitDelay1_DSTATE_fx;
    unsigned char UnitDelay1_DSTATE_bx;
    unsigned char UnitDelay1_DSTATE_o;
    unsigned char UnitDelay1_DSTATE_fz;
    unsigned char UnitDelay1_DSTATE_bh;
    unsigned char UnitDelay_DSTATE_o4;
    unsigned char UnitDelay1_DSTATE_js;
    unsigned char is_active_c15_VcVmcDsr_lib;
    unsigned char is_c15_VcVmcDsr_lib;
    unsigned char is_NORMAL_OPERATION;
    unsigned char is_IN_IDC_SPEED_RANGE;
    unsigned char is_IDC_AVAILABILITY;
    unsigned char is_OUT_OF_IDC_SPEED_RANGE;
    unsigned char is_active_c10_VcVmcDsr_lib;
    unsigned char is_c10_VcVmcDsr_lib;
    struct 
    {
      float t_Timer;
      unsigned char is_active_c5_VcVmcDsr_lib;
      unsigned char is_c5_VcVmcDsr_lib;
      unsigned char is_TRANSFER_dVcVmcDsr_D_DrvlnCm;
    } x4200_MTIDC;
    struct 
    {
      unsigned char is_active_c3_VcVmcDsr_lib;
      unsigned char is_c3_VcVmcDsr_lib;
    } x3230_MTIDC;
    struct 
    {
      unsigned char UnitDelay1_DSTATE;
      unsigned char UnitDelay2_DSTATE;
      unsigned char UnitDelay1_DSTATE_m;
      unsigned char UnitDelay1_DSTATE_p;
      unsigned char UnitDelay1_DSTATE_a;
      unsigned char UnitDelay1_DSTATE_c;
    } x3140_MTIDC;
  } VcVmcDsr_ODC_Present;
} VcVmcDsr_DWork;
static FLAG VcVmcDsr_Idc(void);
static void enter_internal_NORMAL_OPERATION(void);
static FLAG VcVmcDsr_Idc(void)
{
  return sVcVmcPmm_D_DrvlnState == ((U8) 3U);
}

static void enter_internal_NORMAL_OPERATION(void)
{
  VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_NORMAL_OPERATION = (U8) 2U;
  if (! xVcVmcDsr_B_OutOfIdcVehSpdRng)
  {
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IDC_AVAILABILITY = (U8) 1U;
    if (xVcVmcDsr_B_IdcInhbt)
    {
      VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IN_IDC_SPEED_RANGE = (U8) 2U;
      sVcVmcDsr_D_IdcState = (U8) 7U;
    }
    else
    {
      VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IN_IDC_SPEED_RANGE = (U8) 1U;
      sVcVmcDsr_D_IdcState = (U8) 4U;
    }

  }
  else
  {
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IDC_AVAILABILITY = (U8) 2U;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_OUT_OF_IDC_SPEED_RANGE = (U8) 2U;
    sVcVmcDsr_D_IdcState = (U8) 5U;
  }

}

FLAG etime_idc(F32 t0, F32 ti, F32 duration)
{
  FLAG res;
  F32 diff;
  if (t0 <= 0.0F)
  {
    res = 1;
  }
  else
  {
    if (ti < t0)
    {
      diff = (cVcVmcDsr_t_DsrTimerMaxVal - t0) + ti;
    }
    else
    {
      diff = ti - t0;
    }

    res = diff >= duration;
  }

  return res;
}

void VcVmcDsr_step(void)
{
  FLAG xVcVmcDsr_B_NoIdcReqVec[16] = {0};
  FLAG xVcVmcDsr_B_NoIdcReqInhbtVec[32] = {0};
  FLAG xVcVmcDsr_B_NoIdcReq = 0;
  FLAG xVcVmcDsr_B_IdcTerm = 0;
  FLAG xVcVmcDsr_B_IdcNotAvailIdcInhbt = 0;
  FLAG xVcVmcDsr_B_IdcInhbtVec[16] = {0};
  FLAG xVcVmcDsr_B_IdcErr = 0;
  FLAG xVcVmcDsr_B_IdcAccPedPrsd = 0;
  FLAG xVcVmcDsr_B_HystStateTqReqMinusTqMin = 0;
  FLAG xVcVmcDsr_B_HystStateTqReq = 0;
  FLAG xVcVmcDsr_B_HystStateRdGradeBelowNeg = 0;
  FLAG xVcVmcDsr_B_HystStateRdGradeAbovePos = 0;
  FLAG xVcVmcDsr_B_HystStateAccPed = 0;
  F32 t_CurrentIdcDurtn = 0;
  TABLE_CURVE tVcVmcDsr_v_SpdIncIdcTerm_y = {6, 2U, 0x11U, 1U, & tVcVmcDsr_v_SpdIncIdcTerm_x, & tVcVmcDsr_v_SpdIncIdcTerm_y_array[0], {1.0F, 0.0F, 0.0F, 1.0F}};
  TABLE_CURVE tVcVmcDsr_v_CcDeltaIdcTerm_y = {8, 2U, 0x11U, 1U, & tVcVmcDsr_v_CcDeltaIdcTerm_x, & tVcVmcDsr_v_CcDeltaIdcTerm_y_array[0], {1.0F, 0.0F, 0.0F, 1.0F}};
  TABLE_VALBLK tVcVmcDsr_D_IdcValidGears = {12, 1U, 17U, 0, & tVcVmcDsr_D_IdcValidGears_array[0], {1.0F, 0.0F, 0.0F, 1.0F}};
  F32 sVcVmcDsr_rt_IdcDist = 0;
  F32 sVcVmcDsr_Z_IdcDurtn[10] = {0};
  F32 sVcVmcDsr_Z_IdcDist[10] = {0};
  U32 sVcVmcDsr_D_RsnForIdcInhbt = 0;
  U8 sVcVmcDsr_D_DrvlnCmd = 0;
  F32 rVcVmcDsr_v_IdcMin = 0;
  F32 rVcVmcDsr_t_DsrTimer = 0;
  U8 rVcVmcDsr_Z_SpdIncCount = 0;
  U8 rVcVmcDsr_D_VldGear = 0;
  TABLE_VALBLK cVcVmcDsr_n_MinNiIdcTerm = {7, 1U, 17U, 0, & cVcVmcDsr_n_MinNiIdcTerm_array[0], {1.0F, 0.0F, 0.0F, 1.0F}};
  TABLE_VALBLK cVcVmcDsr_n_MaxNiIdcTerm = {7, 1U, 17U, 0, & cVcVmcDsr_n_MaxNiIdcTerm_array[0], {1.0F, 0.0F, 0.0F, 1.0F}};
  const ConstParam_VcVmcDsr VcVmcDsr_ConstP = {0};
  F32 L_IdcEvnt2Evnt = 0;
  F32 L_IdcDist = 0;
  F32 rtb_VariantMergeForOutportts;
  rtb_VariantMergeForOutportts = 0.01F;
  VcVmcDsr_DWork.pvc_fast_ts = rtb_VariantMergeForOutportts;
  {
    F32 rtb_f32min;
    F32 rtb_f32max8;
    F32 rtb_f32max8_j;
    F32 rtb_Sum_ax;
    F32 rtb_f32min_g;
    F32 rtb_f32max;
    F32 rtb_Product1[10];
    F32 rtb_f32max5;
    F32 rtb_Product1_e[10];
    F32 rtb_f32max5_l;
    F32 rtb_tVcVmcDsr_v_NsrCcDelta_y;
    F32 rtb_tVcVmcDsr_v_NsrSpdInc_y;
    F32 rtb_f32max3;
    F32 rtb_f32max3_m;
    F32 rtb_f32max7;
    F32 rtb_f32max7_f;
    F32 rtb_Switch1_j;
    U32 rtb_DataTypeConversion;
    U32 rtb_tVcVmcDsr_v_NsrCcDelta_x;
    U32 rtb_tVcVmcDsr_v_NsrSpdInc_x;
    FLAG rtb_VariantMergeForOutportxVcVm;
    FLAG rtb_VariantMergeForOutportxVc_c;
    FLAG rtb_VariantMergeForOutportxVc_b;
    FLAG rtb_VariantMergeForOutportxVc_k;
    FLAG rtb_VariantMergeForOutportxVc_f;
    FLAG rtb_VariantMergeForOutportxVc_o;
    F32 rtb_Switch[10];
    F32 rtb_Switch1[10];
    F32 rtb_UL;
    F32 rtb_UL1;
    F32 rtb_LL;
    FLAG rtb_B_IdcSelSw;
    FLAG rtb_RelationalOperator1_nk;
    FLAG rtb_UnitDelay1_o;
    FLAG rtb_LogOp3_ju;
    F32 rtb_MultiportSwitch_i;
    F32 rtb_MultiportSwitch;
    U8 rtb_UnitDelay_cs;
    FLAG rtb_LogicalOperator1_i;
    S32 i;
    rtb_LL = cVcVmcDsr_Z_AccPedPrsdThres - cVcVmcDsr_Z_AccPedPrsdHyst;
    if (cVcVmcDsr_B_UseAccPed)
    {
      rtb_UL1 = sVcDi_X_AccPed;
    }
    else
    {
      rtb_UL1 = sVcVmcAjc_Tq_Req;
    }

    rtb_UL = cVcVmcDsr_Z_AccPedPrsdThres + cVcVmcDsr_Z_AccPedPrsdHyst;
    xVcVmcDsr_B_HystStateAccPed = (rtb_UL1 > rtb_UL) || ((! (rtb_UL1 <= rtb_LL)) && VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE_o4);
    xVcVmcDsr_B_HystStateTqReq = xVcVmcDsr_B_HystStateAccPed;
    rtb_UL1 = sVcVmcAjc_Tq_Req - sVcDtcAtf_Tq_Min;
    xVcVmcDsr_B_HystStateTqReqMinusTqMin = (rtb_UL1 > rtb_UL) || ((! (rtb_UL1 <= rtb_LL)) && VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_js);
    if (cVcVmcDsr_B_UseAccPed)
    {
      rtb_B_IdcSelSw = 1;
    }
    else
    {
      rtb_B_IdcSelSw = xVcVmcDsr_B_HystStateTqReqMinusTqMin;
    }

    xVcVmcDsr_B_IdcAccPedPrsd = xVcVmcDsr_B_HystStateAccPed && rtb_B_IdcSelSw;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE_o4 = xVcVmcDsr_B_HystStateAccPed;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_js = xVcVmcDsr_B_HystStateTqReqMinusTqMin;
    rtb_LL = cVcVmcDsr_ts + VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE_e;
    if (! (rtb_LL < cVcVmcDsr_t_DsrTimerMaxVal))
    {
      rtb_LL = cVcVmcDsr_ts;
    }

    rVcVmcDsr_t_DsrTimer = rtb_LL;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE_e = rtb_LL;
    rtb_DataTypeConversion = XXX_D_Gear;
    rtb_Switch1_j = lib_utils_valblk_pick_large(& tVcVmcDsr_D_IdcValidGears, rtb_DataTypeConversion);
    rVcVmcDsr_D_VldGear = (U8) rtb_Switch1_j;
    rtb_VariantMergeForOutportxVc_o = (sVcTc_D_CpedStateChk == ((U8) 0U)) || (XXXTrnGearState_No_Qf == ((U8) 0U));
    xVcVmcDsr_B_IdcErr = (((sVcDi_Qf_AccPed == ((U8) 0U)) || (sVcIfBard_Qf_BpedStat == ((U8) 0U))) || (sVcVmcPmm_D_PtcIdcError != ((U8) 0U))) || rtb_VariantMergeForOutportxVc_o;
    xVcVmcDsr_B_HystStateVehSpdMax = (sVcEc_v_Veh > (cVcVmcDsr_v_DsrMaxVehThres + cVcVmcDsr_v_DsrMaxVehHyst)) || ((! (sVcEc_v_Veh <= (cVcVmcDsr_v_DsrMaxVehThres - cVcVmcDsr_v_DsrMaxVehHyst))) && VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_bh);
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_bh = xVcVmcDsr_B_HystStateVehSpdMax;
    xVcVmcDsr_B_HystStateVehSpdMin = (VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_fz && (! (sVcEc_v_Veh >= (cVcVmcDsr_v_DsrMinVehThres + cVcVmcDsr_v_DsrMinVehHyst)))) || (sVcEc_v_Veh < (cVcVmcDsr_v_DsrMinVehThres - cVcVmcDsr_v_DsrMinVehHyst));
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_fz = xVcVmcDsr_B_HystStateVehSpdMin;
    xVcVmcDsr_B_OutOfIdcVehSpdRng = xVcVmcDsr_B_HystStateVehSpdMax || xVcVmcDsr_B_HystStateVehSpdMin;
    rtb_B_IdcSelSw = (yVcDi_B_IdcSelSw == 0) && cVcVmcDsr_B_IdcSelSwAsIdcTerm;
    {
      F32 rtb_pick_valblk1;
      F32 rtb_pick_valblk2;
      U32 rtb_DataTypeConversion3;
      FLAG rtb_UnitDelay1_i;
      FLAG rtb_LogOp3_h4;
      FLAG rtb_LogOp3_km;
      FLAG rtb_LogOp3_n;
      rtb_DataTypeConversion3 = rVcVmcDsr_D_VldGear;
      rtb_pick_valblk1 = lib_utils_valblk_pick_large(& cVcVmcDsr_n_MaxNiIdcTerm, rtb_DataTypeConversion3);
      rtb_DataTypeConversion3 = rVcVmcDsr_D_VldGear;
      rtb_pick_valblk2 = lib_utils_valblk_pick_large(& cVcVmcDsr_n_MinNiIdcTerm, rtb_DataTypeConversion3);
      rtb_UnitDelay1_i = (sVcTc_n_GearBoxIn >= (rtb_pick_valblk2 + cVcVmcDsr_n_HystNiIdcTerm)) && (sVcTc_n_GearBoxIn <= (rtb_pick_valblk1 - cVcVmcDsr_n_HystNiIdcTerm));
      rtb_LogOp3_h4 = (((sVcTc_n_GearBoxIn >= (rtb_pick_valblk1 + cVcVmcDsr_n_HystNiIdcTerm)) || (sVcTc_n_GearBoxIn <= (rtb_pick_valblk2 - cVcVmcDsr_n_HystNiIdcTerm))) && (! rtb_UnitDelay1_i)) || ((! rtb_UnitDelay1_i) && VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE_m);
      rtb_VariantMergeForOutportxVc_b = rtb_LogOp3_h4;
      rtb_UnitDelay1_i = (sVcTc_D_CpedState == ((U8) 0U)) && xVcVmcDsr_B_IdcAccPedPrsd;
      rtb_UnitDelay1_i = (((sVcTc_D_CpedState != ((U8) 0U)) && cVcVmcDsr_B_IdcTermClPed) && (! rtb_UnitDelay1_i)) || ((! rtb_UnitDelay1_i) && VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE_p);
      rtb_VariantMergeForOutportxVcVm = rtb_UnitDelay1_i;
      rtb_LogOp3_km = ((((sVcTc_D_CpedState == ((U8) 0U)) && (XXX_D_Gear != VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE)) && cVcVmcDsr_B_IdcTermGearChngdWoClPed) && (! xVcVmcDsr_B_IdcAccPedPrsd)) || ((! xVcVmcDsr_B_IdcAccPedPrsd) && VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE_a);
      rtb_VariantMergeForOutportxVc_k = rtb_LogOp3_km;
      if (rVcVmcDsr_D_VldGear != cVcVmcDsr_D_GearNtrl)
      {
        VcVmcDsr_B.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay2 = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay2_DSTATE;
        VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay2_DSTATE = rVcVmcDsr_D_VldGear;
      }

      rtb_LogOp3_n = ((((VcVmcDsr_B.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay2 > rVcVmcDsr_D_VldGear) && (rVcVmcDsr_D_VldGear != cVcVmcDsr_D_GearNtrl)) && cVcVmcDsr_B_IdcTermGearDnshft) && (! xVcVmcDsr_B_IdcAccPedPrsd)) || ((! xVcVmcDsr_B_IdcAccPedPrsd) && VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE_c);
      rtb_VariantMergeForOutportxVc_c = rtb_LogOp3_n;
      switch (XXX_D_Gear)
      {
        case 0:
          rtb_VariantMergeForOutportxVc_f = tVcVmcDsr_B_GearAsIdcTerm[0];
          break;

        case 1:
          rtb_VariantMergeForOutportxVc_f = tVcVmcDsr_B_GearAsIdcTerm[1];
          break;

        case 2:
          rtb_VariantMergeForOutportxVc_f = tVcVmcDsr_B_GearAsIdcTerm[2];
          break;

        case 3:
          rtb_VariantMergeForOutportxVc_f = tVcVmcDsr_B_GearAsIdcTerm[3];
          break;

        case 4:
          rtb_VariantMergeForOutportxVc_f = tVcVmcDsr_B_GearAsIdcTerm[4];
          break;

        case 5:
          rtb_VariantMergeForOutportxVc_f = tVcVmcDsr_B_GearAsIdcTerm[5];
          break;

        case 6:
          rtb_VariantMergeForOutportxVc_f = tVcVmcDsr_B_GearAsIdcTerm[6];
          break;

        case 7:
          rtb_VariantMergeForOutportxVc_f = tVcVmcDsr_B_GearAsIdcTerm[7];
          break;

        case 8:
          rtb_VariantMergeForOutportxVc_f = tVcVmcDsr_B_GearAsIdcTerm[8];
          break;

        case 9:
          rtb_VariantMergeForOutportxVc_f = tVcVmcDsr_B_GearAsIdcTerm[9];
          break;

        case 10:
          rtb_VariantMergeForOutportxVc_f = tVcVmcDsr_B_GearAsIdcTerm[10];
          break;

        default:
          rtb_VariantMergeForOutportxVc_f = tVcVmcDsr_B_GearAsIdcTerm[11];
          break;

      }

      VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE_m = rtb_LogOp3_h4;
      VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE_p = rtb_UnitDelay1_i;
      VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE = XXX_D_Gear;
      VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE_a = rtb_LogOp3_km;
      VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE_c = rtb_LogOp3_n;
    }
    rtb_RelationalOperator1_nk = yVcIfBard_B_BpedStat == 1;
    rtb_UnitDelay1_o = (! rtb_RelationalOperator1_nk) && (xVcVmcDsr_B_IdcAccPedPrsd || ((sVcTc_D_CpedState != ((U8) 0U)) && (! cVcVmcDsr_B_IdcTermClPed)));
    rtb_RelationalOperator1_nk = (rtb_RelationalOperator1_nk && (! rtb_UnitDelay1_o)) || ((! rtb_UnitDelay1_o) && VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_fx);
    rtb_UnitDelay_cs = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE_ew;
    if (xVcVmcDsr_B_IdcAccPedPrsd)
    {
      rVcVmcDsr_v_IdcMin = sVcEc_v_Veh;
    }
    else
    {
      rtb_Sum_ax = ((1.0F - cVcVmcDsr_Z_SpdIncFltCnst) * sVcEc_v_Veh) + (cVcVmcDsr_Z_SpdIncFltCnst * VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE);
      rtb_f32min_g = lib_utils_minimum_f32(sVcEc_v_Veh, rtb_Sum_ax);
      rVcVmcDsr_v_IdcMin = rtb_f32min_g;
    }

    if (! xVcVmcDsr_B_IdcAccPedPrsd)
    {
      rtb_tVcVmcDsr_v_NsrSpdInc_x = lib_utils_axis_large(& tVcVmcDsr_v_SpdIncIdcTerm_x, sVcEc_v_Veh);
      rtb_tVcVmcDsr_v_NsrSpdInc_y = lib_utils_curve_large(& tVcVmcDsr_v_SpdIncIdcTerm_y, rtb_tVcVmcDsr_v_NsrSpdInc_x);
      if (sVcEc_v_Veh > (rtb_tVcVmcDsr_v_NsrSpdInc_y + rVcVmcDsr_v_IdcMin))
      {
        rtb_UnitDelay_cs++;
        if (! (rtb_UnitDelay_cs <= cVcVmcDsr_Z_SpdIncCountLim))
        {
          rtb_UnitDelay_cs = cVcVmcDsr_Z_SpdIncCountLim;
        }

      }
      else
      {
        if (sVcEc_v_Veh <= (rVcVmcDsr_v_IdcMin + cVcVmcDsr_v_SpdIncMinVehTol))
        {
          if (! (rtb_UnitDelay_cs >= 1))
          {
            rtb_UnitDelay_cs = 1U;
          }

          rtb_UnitDelay_cs--;
        }

      }

      rVcVmcDsr_Z_SpdIncCount = rtb_UnitDelay_cs;
    }
    else
    {
      rVcVmcDsr_Z_SpdIncCount = 0U;
    }

    rtb_UnitDelay1_o = ((rVcVmcDsr_Z_SpdIncCount <= 0) && cVcVmcDsr_B_SpdIncRstIdcTerm) || xVcVmcDsr_B_IdcAccPedPrsd;
    rtb_LogOp3_ju = ((rVcVmcDsr_Z_SpdIncCount >= cVcVmcDsr_Z_SpdIncCountLim) && (! rtb_UnitDelay1_o)) || ((! rtb_UnitDelay1_o) && VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_bx);
    rtb_UnitDelay1_o = sVcDeScl_D_CcStatus == ((U8) 5U);
    rtb_tVcVmcDsr_v_NsrCcDelta_x = lib_utils_axis_large(& tVcVmcDsr_v_CcDeltaIdcTerm_x, sVcEc_v_Veh);
    rtb_tVcVmcDsr_v_NsrCcDelta_y = lib_utils_curve_large(& tVcVmcDsr_v_CcDeltaIdcTerm_y, rtb_tVcVmcDsr_v_NsrCcDelta_x);
    rtb_UnitDelay1_o = (rtb_UnitDelay1_o && ((! cVcVmcDsr_B_CcIdcAllow) || (sVcEc_v_Veh > (sVcDeScl_v_CcSet + rtb_tVcVmcDsr_v_NsrCcDelta_y)))) || ((! ((! rtb_UnitDelay1_o) || (cVcVmcDsr_B_CcIdcAllow && (sVcEc_v_Veh <= sVcDeScl_v_CcSet)))) && VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_o);
    xVcVmcDsr_B_NoIdcReqVec[0] = xVcVmcDsr_B_OutOfIdcVehSpdRng;
    xVcVmcDsr_B_NoIdcReqVec[1] = rtb_B_IdcSelSw;
    xVcVmcDsr_B_NoIdcReqVec[2] = 0;
    xVcVmcDsr_B_NoIdcReqVec[3] = rtb_VariantMergeForOutportxVc_f;
    xVcVmcDsr_B_NoIdcReqVec[4] = rtb_VariantMergeForOutportxVc_b;
    xVcVmcDsr_B_NoIdcReqVec[5] = rtb_RelationalOperator1_nk;
    xVcVmcDsr_B_NoIdcReqVec[6] = rtb_LogOp3_ju;
    xVcVmcDsr_B_NoIdcReqVec[7] = rtb_UnitDelay1_o;
    xVcVmcDsr_B_NoIdcReqVec[8] = 0;
    xVcVmcDsr_B_NoIdcReqVec[9] = TBD_DrvSituationDetection_NoIdcReq;
    xVcVmcDsr_B_NoIdcReqVec[10] = TBD_Spare_NoIdcReq;
    xVcVmcDsr_B_NoIdcReqVec[11] = 0;
    xVcVmcDsr_B_NoIdcReqVec[12] = rtb_VariantMergeForOutportxVc_k;
    xVcVmcDsr_B_NoIdcReqVec[13] = rtb_VariantMergeForOutportxVcVm;
    xVcVmcDsr_B_NoIdcReqVec[14] = rtb_VariantMergeForOutportxVc_c;
    xVcVmcDsr_B_NoIdcReqVec[15] = xVcVmcDsr_B_IdcErr;
    rtb_B_IdcSelSw = ((((((((((rtb_B_IdcSelSw || rtb_VariantMergeForOutportxVc_f) || rtb_VariantMergeForOutportxVc_b) || rtb_RelationalOperator1_nk) || rtb_LogOp3_ju) || rtb_UnitDelay1_o) || TBD_DrvSituationDetection_NoIdcReq) || TBD_Spare_NoIdcReq) || xVcVmcDsr_B_IdcErr) || rtb_VariantMergeForOutportxVc_k) || rtb_VariantMergeForOutportxVcVm) || rtb_VariantMergeForOutportxVc_c;
    xVcVmcDsr_B_NoIdcReq = xVcVmcDsr_B_OutOfIdcVehSpdRng || rtb_B_IdcSelSw;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_fx = rtb_RelationalOperator1_nk;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE_ew = rVcVmcDsr_Z_SpdIncCount;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE = rVcVmcDsr_v_IdcMin;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_bx = rtb_LogOp3_ju;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_o = rtb_UnitDelay1_o;
    rtb_RelationalOperator1_nk = yVcDi_B_IdcSelSw == 0;
    rtb_LogOp3_ju = sVcIfRge_Qf_RdGrade == ((U8) 3U);
    xVcVmcDsr_B_HystStateRdGradeBelowNeg = (VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_e && (! (sVcIfRge_X_RdGrade >= (cVcVmcDsr_X_RdGradeNegIdcInhThres + cVcVmcDsr_X_RdGradeIdcInhHyst)))) || (sVcIfRge_X_RdGrade < (cVcVmcDsr_X_RdGradeNegIdcInhThres - cVcVmcDsr_X_RdGradeIdcInhHyst));
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_e = xVcVmcDsr_B_HystStateRdGradeBelowNeg;
    xVcVmcDsr_B_HystStateRdGradeAbovePos = (sVcIfRge_X_RdGrade > (cVcVmcDsr_X_RdGradePosIdcInhThres + cVcVmcDsr_X_RdGradeIdcInhHyst)) || ((! (sVcIfRge_X_RdGrade <= (cVcVmcDsr_X_RdGradePosIdcInhThres - cVcVmcDsr_X_RdGradeIdcInhHyst))) && VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_b);
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_b = xVcVmcDsr_B_HystStateRdGradeAbovePos;
    rtb_UnitDelay1_o = (sVcIfRge_X_RdGrade > (cVcVmcDsr_X_RdGradeNegIdcInhThres + cVcVmcDsr_X_RdGradeIdcInhHyst)) || ((! (sVcIfRge_X_RdGrade <= (cVcVmcDsr_X_RdGradeNegIdcInhThres - cVcVmcDsr_X_RdGradeIdcInhHyst))) && VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_fe);
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_fe = rtb_UnitDelay1_o;
    rtb_LogicalOperator1_i = (VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_f && (! (sVcIfRge_X_RdGrade >= (cVcVmcDsr_X_RdGradePosIdcInhThres + cVcVmcDsr_X_RdGradeIdcInhHyst)))) || (sVcIfRge_X_RdGrade < (cVcVmcDsr_X_RdGradePosIdcInhThres - cVcVmcDsr_X_RdGradeIdcInhHyst));
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_f = rtb_LogicalOperator1_i;
    rtb_LogOp3_ju = ((xVcVmcDsr_B_HystStateRdGradeBelowNeg || xVcVmcDsr_B_HystStateRdGradeAbovePos) && rtb_LogOp3_ju) || ((! ((! rtb_LogOp3_ju) || (rtb_UnitDelay1_o && rtb_LogicalOperator1_i))) && VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_d);
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_d = rtb_LogOp3_ju;
    rtb_UnitDelay1_o = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE == ((U8) 3U);
    xVcVmcDsr_B_IdcTerm = (! rtb_UnitDelay1_o) && VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE_o;
    if (VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_active_c10_VcVmcDsr_lib == 0U)
    {
      VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_active_c10_VcVmcDsr_lib = 1U;
      VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_c10_VcVmcDsr_lib = (U8) 1U;
      VcVmcDsr_B.VcVmcDsr_ODC_Present.xVcVmcDsr_B_IdclInhbtAftrDrvlnC = 0;
    }
    else
      if (VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_c10_VcVmcDsr_lib == ((U8) 1U))
    {
      if (xVcVmcDsr_B_IdcTerm && (cVcVmcDsr_t_TermIdcInhTout > 0.0F))
      {
        VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_c10_VcVmcDsr_lib = (U8) 2U;
        VcVmcDsr_B.VcVmcDsr_ODC_Present.xVcVmcDsr_B_IdclInhbtAftrDrvlnC = 1;
        VcVmcDsr_DWork.VcVmcDsr_ODC_Present.t_Timer = rVcVmcDsr_t_DsrTimer;
      }

    }
    else
    {
      if (etime_idc(VcVmcDsr_DWork.VcVmcDsr_ODC_Present.t_Timer, rVcVmcDsr_t_DsrTimer, cVcVmcDsr_t_TermIdcInhTout))
      {
        VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_c10_VcVmcDsr_lib = (U8) 1U;
        VcVmcDsr_B.VcVmcDsr_ODC_Present.xVcVmcDsr_B_IdclInhbtAftrDrvlnC = 0;
      }

    }


    if (VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3230_MTIDC.is_active_c3_VcVmcDsr_lib == 0U)
    {
      VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3230_MTIDC.is_active_c3_VcVmcDsr_lib = 1U;
      VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3230_MTIDC.is_c3_VcVmcDsr_lib = (U8) 1U;
      VcVmcDsr_B.VcVmcDsr_ODC_Present.VariantMergeForOutportxVcVmcDsr = 0;
    }
    else
      if (VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3230_MTIDC.is_c3_VcVmcDsr_lib == ((U8) 1U))
    {
      if ((cVcVmcDsr_B_TermIdcInhClrPed || cVcVmcDsr_B_TermIdcInhClrPedCl) && xVcVmcDsr_B_IdcTerm)
      {
        VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3230_MTIDC.is_c3_VcVmcDsr_lib = (U8) 2U;
        VcVmcDsr_B.VcVmcDsr_ODC_Present.VariantMergeForOutportxVcVmcDsr = 1;
      }

    }
    else
    {
      if (((sVcTc_D_CpedState != ((U8) 0U)) && cVcVmcDsr_B_TermIdcInhClrPedCl) || (xVcVmcDsr_B_IdcAccPedPrsd && cVcVmcDsr_B_TermIdcInhClrPed))
      {
        VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3230_MTIDC.is_c3_VcVmcDsr_lib = (U8) 1U;
        VcVmcDsr_B.VcVmcDsr_ODC_Present.VariantMergeForOutportxVcVmcDsr = 0;
      }

    }


    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE_o = rtb_UnitDelay1_o;
    if (VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay3_DSTATE_o)
    {
      rtb_LL = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE_h + cVcVmcDsr_ts;
    }
    else
    {
      rtb_LL = 0.0F;
    }

    rtb_UnitDelay1_o = (xVcVmcDsr_B_IdcAccPedPrsd && (! VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_ne)) || (rtb_LL > cVcVmcDsr_t_AccPedGradTimeOut);
    rtb_UnitDelay1_o = ((sVcDi_Xd_AccPed < cVcVmcDsr_Xd_AccPedIdcInh) && (! rtb_UnitDelay1_o)) || ((! rtb_UnitDelay1_o) && VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_j);
    xVcVmcDsr_B_IdcInhbtVec[0] = rtb_RelationalOperator1_nk;
    xVcVmcDsr_B_IdcInhbtVec[1] = rtb_LogOp3_ju;
    xVcVmcDsr_B_IdcInhbtVec[2] = VcVmcDsr_B.VcVmcDsr_ODC_Present.xVcVmcDsr_B_IdclInhbtAftrDrvlnC;
    xVcVmcDsr_B_IdcInhbtVec[3] = VcVmcDsr_B.VcVmcDsr_ODC_Present.VariantMergeForOutportxVcVmcDsr;
    xVcVmcDsr_B_IdcInhbtVec[4] = rtb_UnitDelay1_o;
    xVcVmcDsr_B_IdcInhbtVec[5] = 0;
    xVcVmcDsr_B_IdcInhbtVec[6] = 0;
    xVcVmcDsr_B_IdcInhbtVec[7] = 0;
    xVcVmcDsr_B_IdcInhbtVec[8] = 0;
    xVcVmcDsr_B_IdcInhbtVec[9] = TBD_DrvSituationDetection_IdcInhbt;
    xVcVmcDsr_B_IdcInhbtVec[10] = TBD_Spare_IdcInhbt;
    xVcVmcDsr_B_IdcInhbtVec[11] = 0;
    xVcVmcDsr_B_IdcInhbtVec[12] = 0;
    xVcVmcDsr_B_IdcInhbtVec[13] = 0;
    xVcVmcDsr_B_IdcInhbtVec[14] = 0;
    xVcVmcDsr_B_IdcInhbtVec[15] = 0;
    for (i = 0; i < 16; i++)
    {
      xVcVmcDsr_B_NoIdcReqInhbtVec[i] = xVcVmcDsr_B_NoIdcReqVec[i];
      xVcVmcDsr_B_NoIdcReqInhbtVec[i + 16] = xVcVmcDsr_B_IdcInhbtVec[i];
    }

    xVcVmcDsr_B_IdcNotAvailIdcInhbt = ((((((rtb_RelationalOperator1_nk || rtb_B_IdcSelSw) || rtb_LogOp3_ju) || VcVmcDsr_B.VcVmcDsr_ODC_Present.xVcVmcDsr_B_IdclInhbtAftrDrvlnC) || VcVmcDsr_B.VcVmcDsr_ODC_Present.VariantMergeForOutportxVcVmcDsr) || TBD_DrvSituationDetection_IdcInhbt) || TBD_Spare_IdcInhbt) || rtb_UnitDelay1_o;
    xVcVmcDsr_B_IdcInhbt = xVcVmcDsr_B_NoIdcReq || xVcVmcDsr_B_IdcNotAvailIdcInhbt;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_ne = xVcVmcDsr_B_IdcAccPedPrsd;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE_h = rtb_LL;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay3_DSTATE_o = rtb_UnitDelay1_o;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_j = rtb_UnitDelay1_o;
    if (VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_active_c5_VcVmcDsr_lib == 0U)
    {
      VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_active_c5_VcVmcDsr_lib = 1U;
      VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_c5_VcVmcDsr_lib = (U8) 2U;
      sVcVmcDsr_D_DrvlnCmd = (U8) 1U;
    }
    else
    {
      switch (VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_c5_VcVmcDsr_lib)
      {
        case (U8) 1U:
          if ((sVcVmcPmm_D_DrvlnState == ((U8) 3U)) && xVcVmcDsr_B_NoIdcReq)
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_c5_VcVmcDsr_lib = (U8) 2U;
          sVcVmcDsr_D_DrvlnCmd = (U8) 1U;
        }
        else
        {
          if ((sVcVmcPmm_D_DrvlnState == ((U8) 3U)) && xVcVmcDsr_B_IdcAccPedPrsd)
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_c5_VcVmcDsr_lib = (U8) 3U;
            sVcVmcDsr_D_DrvlnCmd = (U8) 2U;
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_TRANSFER_dVcVmcDsr_D_DrvlnCm = (U8) 1U;
          }

        }

          break;

        case (U8) 2U:
          if ((sVcVmcPmm_D_DrvlnState == ((U8) 1U)) && (! xVcVmcDsr_B_IdcInhbt))
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_c5_VcVmcDsr_lib = (U8) 3U;
          sVcVmcDsr_D_DrvlnCmd = (U8) 2U;
          if (xVcVmcDsr_B_IdcAccPedPrsd || (sVcTc_D_CpedState != ((U8) 0U)))
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_TRANSFER_dVcVmcDsr_D_DrvlnCm = (U8) 1U;
          }
          else
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_TRANSFER_dVcVmcDsr_D_DrvlnCm = (U8) 2U;
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.t_Timer = rVcVmcDsr_t_DsrTimer;
          }

        }

          break;

        default:
          if ((sVcVmcPmm_D_DrvlnState == ((U8) 2U)) && xVcVmcDsr_B_IdcInhbt)
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_TRANSFER_dVcVmcDsr_D_DrvlnCm = (U8) 0U;
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_c5_VcVmcDsr_lib = (U8) 2U;
          sVcVmcDsr_D_DrvlnCmd = (U8) 1U;
        }
        else
          if (VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_TRANSFER_dVcVmcDsr_D_DrvlnCm == ((U8) 1U))
        {
          if ((! xVcVmcDsr_B_IdcAccPedPrsd) && (sVcTc_D_CpedState == ((U8) 0U)))
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_TRANSFER_dVcVmcDsr_D_DrvlnCm = (U8) 2U;
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.t_Timer = rVcVmcDsr_t_DsrTimer;
          }

        }
        else
        {
          if (xVcVmcDsr_B_IdcAccPedPrsd || (sVcTc_D_CpedState != ((U8) 0U)))
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_TRANSFER_dVcVmcDsr_D_DrvlnCm = (U8) 1U;
          }
          else
          {
            if ((sVcVmcPmm_D_DrvlnState == ((U8) 2U)) && etime_idc(VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.t_Timer, rVcVmcDsr_t_DsrTimer, cVcVmcDsr_t_IdcDelay))
            {
              VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_TRANSFER_dVcVmcDsr_D_DrvlnCm = (U8) 0U;
              VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_c5_VcVmcDsr_lib = (U8) 1U;
              sVcVmcDsr_D_DrvlnCmd = (U8) 3U;
            }

          }

        }


          break;

      }

    }

    if (VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_active_c15_VcVmcDsr_lib == 0U)
    {
      VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_active_c15_VcVmcDsr_lib = 1U;
      VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_c15_VcVmcDsr_lib = (U8) 2U;
      enter_internal_NORMAL_OPERATION();
    }
    else
      if (VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_c15_VcVmcDsr_lib == ((U8) 1U))
    {
      if (! xVcVmcDsr_B_IdcErr)
      {
        VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_c15_VcVmcDsr_lib = (U8) 2U;
        enter_internal_NORMAL_OPERATION();
      }

    }
    else
    {
      if (xVcVmcDsr_B_IdcErr)
      {
        VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_NORMAL_OPERATION = 0U;
        VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IN_IDC_SPEED_RANGE = 0U;
        VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IDC_AVAILABILITY = 0U;
        VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_OUT_OF_IDC_SPEED_RANGE = 0U;
        VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_c15_VcVmcDsr_lib = (U8) 1U;
        sVcVmcDsr_D_IdcState = (U8) 6U;
      }
      else
        if (VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_NORMAL_OPERATION == ((U8) 1U))
      {
        if (! VcVmcDsr_Idc())
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_NORMAL_OPERATION = (U8) 2U;
          if (! xVcVmcDsr_B_OutOfIdcVehSpdRng)
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IDC_AVAILABILITY = (U8) 1U;
            if (xVcVmcDsr_B_IdcInhbt)
            {
              VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IN_IDC_SPEED_RANGE = (U8) 2U;
              sVcVmcDsr_D_IdcState = (U8) 7U;
            }
            else
            {
              VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IN_IDC_SPEED_RANGE = (U8) 1U;
              sVcVmcDsr_D_IdcState = (U8) 4U;
            }

          }
          else
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IDC_AVAILABILITY = (U8) 2U;
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_OUT_OF_IDC_SPEED_RANGE = (U8) 2U;
            sVcVmcDsr_D_IdcState = (U8) 5U;
          }

        }

      }
      else
      {
        if (VcVmcDsr_Idc())
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IN_IDC_SPEED_RANGE = 0U;
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IDC_AVAILABILITY = 0U;
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_OUT_OF_IDC_SPEED_RANGE = 0U;
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_NORMAL_OPERATION = (U8) 1U;
          sVcVmcDsr_D_IdcState = (U8) 3U;
        }
        else
          if (VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IDC_AVAILABILITY == ((U8) 1U))
        {
          if (VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IN_IDC_SPEED_RANGE == ((U8) 1U))
          {
            if (xVcVmcDsr_B_OutOfIdcVehSpdRng)
            {
              VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IN_IDC_SPEED_RANGE = 0U;
              VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IDC_AVAILABILITY = (U8) 2U;
              VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_OUT_OF_IDC_SPEED_RANGE = (U8) 2U;
              sVcVmcDsr_D_IdcState = (U8) 5U;
            }
            else
            {
              if (xVcVmcDsr_B_IdcInhbt)
              {
                VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IN_IDC_SPEED_RANGE = (U8) 2U;
                sVcVmcDsr_D_IdcState = (U8) 7U;
              }
              else
              {
                VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IN_IDC_SPEED_RANGE = (U8) 1U;
                sVcVmcDsr_D_IdcState = (U8) 4U;
              }

            }

          }
          else
          {
            if (xVcVmcDsr_B_OutOfIdcVehSpdRng)
            {
              VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IN_IDC_SPEED_RANGE = 0U;
              VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IDC_AVAILABILITY = (U8) 2U;
              VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_OUT_OF_IDC_SPEED_RANGE = (U8) 1U;
              sVcVmcDsr_D_IdcState = (U8) 7U;
            }
            else
            {
              if (xVcVmcDsr_B_IdcInhbt)
              {
                VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IN_IDC_SPEED_RANGE = (U8) 2U;
                sVcVmcDsr_D_IdcState = (U8) 7U;
              }
              else
              {
                VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IN_IDC_SPEED_RANGE = (U8) 1U;
                sVcVmcDsr_D_IdcState = (U8) 4U;
              }

            }

          }

        }
        else
        {
          if (! xVcVmcDsr_B_OutOfIdcVehSpdRng)
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_OUT_OF_IDC_SPEED_RANGE = 0U;
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IDC_AVAILABILITY = (U8) 1U;
            if (xVcVmcDsr_B_IdcInhbt)
            {
              VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IN_IDC_SPEED_RANGE = (U8) 2U;
              sVcVmcDsr_D_IdcState = (U8) 7U;
            }
            else
            {
              VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IN_IDC_SPEED_RANGE = (U8) 1U;
              sVcVmcDsr_D_IdcState = (U8) 4U;
            }

          }
          else
            if ((VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_OUT_OF_IDC_SPEED_RANGE == ((U8) 1U)) && (! xVcVmcDsr_B_IdcNotAvailIdcInhbt))
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_OUT_OF_IDC_SPEED_RANGE = (U8) 2U;
            sVcVmcDsr_D_IdcState = (U8) 5U;
          }
          else
          {
          }


        }


      }


    }


    sVcVmcDsr_D_RsnForIdcInhbt = 0U;
    for (rtb_UnitDelay_cs = 0U; rtb_UnitDelay_cs < 32; rtb_UnitDelay_cs++)
    {
      if (xVcVmcDsr_B_NoIdcReqInhbtVec[rtb_UnitDelay_cs])
      {
        if (cVcVmcDsr_D_IdcIhbtClassAsgnmt[rtb_UnitDelay_cs] != 0)
        {
          sVcVmcDsr_D_RsnForIdcInhbt = (0x00000001 << (cVcVmcDsr_D_IdcIhbtClassAsgnmt[rtb_UnitDelay_cs] - 1)) | sVcVmcDsr_D_RsnForIdcInhbt;
        }
        else
        {
        }

      }
      else
      {
      }

    }

    rtb_LL = (sVcEc_v_Veh / 3.6F) * cVcVmcDsr_ts;
    rtb_B_IdcSelSw = sVcVmcDsr_D_DrvlnCmd == ((U8) 3U);
    rtb_RelationalOperator1_nk = rtb_B_IdcSelSw && (! VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay5_DSTATE);
    if (rtb_RelationalOperator1_nk)
    {
      rtb_Switch1_j = 0.0F;
    }
    else
    {
      if (rtb_B_IdcSelSw)
      {
        rtb_UL1 = rtb_LL;
      }
      else
      {
        rtb_UL1 = 0.0F;
      }

      rtb_Switch1_j = rtb_UL1 + VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE;
    }

    L_IdcDist = lib_utils_minimum_f32(1.0E+30F, rtb_Switch1_j);
    if (rtb_RelationalOperator1_nk)
    {
      rtb_Switch1_j = 0.0F;
    }
    else
    {
      if (rtb_B_IdcSelSw)
      {
        rtb_UL1 = cVcVmcDsr_ts;
      }
      else
      {
        rtb_UL1 = 0.0F;
      }

      rtb_Switch1_j = rtb_UL1 + VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_n;
    }

    t_CurrentIdcDurtn = lib_utils_minimum_f32(1.0E+30F, rtb_Switch1_j);
    rtb_RelationalOperator1_nk = (! rtb_B_IdcSelSw) && VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_n;
    if (rtb_RelationalOperator1_nk)
    {
      rtb_Switch1_j = 0.0F;
    }
    else
    {
      rtb_Switch1_j = rtb_LL + VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e;
    }

    rtb_f32min = lib_utils_minimum_f32(1.0E+30F, rtb_Switch1_j);
    if (rtb_RelationalOperator1_nk)
    {
      rtb_Product1_e[0] = 0.0F;
      for (i = 0; i < 9; i++)
      {
        rtb_Product1_e[i + 1] = (t_CurrentIdcDurtn > cVcVmcDsr_t_ClassIdcDurtn[i]) ? (VcVmcDsr_ConstP.pooled4[i + 1]) : (0.0F);
      }

      rtb_f32max3_m = lib_utils_maximum_f32(rtb_Product1_e[0], rtb_Product1_e[1]);
      rtb_f32max7_f = lib_utils_maximum_f32(rtb_Product1_e[2], rtb_Product1_e[3]);
      rtb_f32max5_l = lib_utils_maximum_f32(rtb_f32max3_m, rtb_f32max7_f);
      rtb_f32max7_f = lib_utils_maximum_f32(rtb_Product1_e[4], rtb_Product1_e[5]);
      rtb_f32max3_m = lib_utils_maximum_f32(rtb_Product1_e[6], rtb_Product1_e[7]);
      rtb_MultiportSwitch_i = lib_utils_maximum_f32(rtb_f32max7_f, rtb_f32max3_m);
      rtb_f32max7_f = lib_utils_maximum_f32(rtb_f32max5_l, rtb_MultiportSwitch_i);
      rtb_MultiportSwitch_i = lib_utils_maximum_f32(rtb_Product1_e[8], rtb_Product1_e[9]);
      rtb_f32max8_j = lib_utils_maximum_f32(rtb_f32max7_f, rtb_MultiportSwitch_i);
      switch ((S32) rtb_f32max8_j)
      {
        case 0:
          rtb_MultiportSwitch_i = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[0];
          break;

        case 1:
          rtb_MultiportSwitch_i = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[1];
          break;

        case 2:
          rtb_MultiportSwitch_i = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[2];
          break;

        case 3:
          rtb_MultiportSwitch_i = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[3];
          break;

        case 4:
          rtb_MultiportSwitch_i = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[4];
          break;

        case 5:
          rtb_MultiportSwitch_i = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[5];
          break;

        case 6:
          rtb_MultiportSwitch_i = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[6];
          break;

        case 7:
          rtb_MultiportSwitch_i = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[7];
          break;

        case 8:
          rtb_MultiportSwitch_i = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[8];
          break;

        default:
          rtb_MultiportSwitch_i = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[9];
          break;

      }

      for (i = 0; i < 10; i++)
      {
        VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[i] = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[i];
      }

      VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[(S32) rtb_f32max8_j] = rtb_MultiportSwitch_i + 1.0F;
    }

    for (i = 0; i < 10; i++)
    {
      if (cVcVmcDsr_B_DsrStatsRst)
      {
        rtb_LL = 0.0F;
      }
      else
      {
        rtb_LL = VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[i];
      }

      sVcVmcDsr_Z_IdcDurtn[i] = rtb_LL;
      rtb_Switch[i] = rtb_LL;
    }

    if (rtb_RelationalOperator1_nk)
    {
      rtb_Product1[0] = 0.0F;
      for (i = 0; i < 9; i++)
      {
        rtb_Product1[i + 1] = (L_IdcDist > cVcVmcDsr_L_ClassIdcDist[i]) ? (VcVmcDsr_ConstP.pooled4[i + 1]) : (0.0F);
      }

      rtb_f32max3 = lib_utils_maximum_f32(rtb_Product1[0], rtb_Product1[1]);
      rtb_f32max7 = lib_utils_maximum_f32(rtb_Product1[2], rtb_Product1[3]);
      rtb_f32max5 = lib_utils_maximum_f32(rtb_f32max3, rtb_f32max7);
      rtb_f32max7 = lib_utils_maximum_f32(rtb_Product1[4], rtb_Product1[5]);
      rtb_f32max3 = lib_utils_maximum_f32(rtb_Product1[6], rtb_Product1[7]);
      rtb_MultiportSwitch = lib_utils_maximum_f32(rtb_f32max7, rtb_f32max3);
      rtb_f32max7 = lib_utils_maximum_f32(rtb_f32max5, rtb_MultiportSwitch);
      rtb_MultiportSwitch = lib_utils_maximum_f32(rtb_Product1[8], rtb_Product1[9]);
      rtb_f32max8 = lib_utils_maximum_f32(rtb_f32max7, rtb_MultiportSwitch);
      switch ((S32) rtb_f32max8)
      {
        case 0:
          rtb_MultiportSwitch = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[0];
          break;

        case 1:
          rtb_MultiportSwitch = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[1];
          break;

        case 2:
          rtb_MultiportSwitch = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[2];
          break;

        case 3:
          rtb_MultiportSwitch = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[3];
          break;

        case 4:
          rtb_MultiportSwitch = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[4];
          break;

        case 5:
          rtb_MultiportSwitch = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[5];
          break;

        case 6:
          rtb_MultiportSwitch = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[6];
          break;

        case 7:
          rtb_MultiportSwitch = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[7];
          break;

        case 8:
          rtb_MultiportSwitch = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[8];
          break;

        default:
          rtb_MultiportSwitch = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[9];
          break;

      }

      for (i = 0; i < 10; i++)
      {
        VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[i] = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[i];
      }

      VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[(S32) rtb_f32max8] = rtb_MultiportSwitch + 1.0F;
    }

    for (i = 0; i < 10; i++)
    {
      if (cVcVmcDsr_B_DsrStatsRst)
      {
        rtb_MultiportSwitch = 0.0F;
      }
      else
      {
        rtb_MultiportSwitch = VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[i];
      }

      sVcVmcDsr_Z_IdcDist[i] = rtb_MultiportSwitch;
      rtb_Switch1[i] = rtb_MultiportSwitch;
    }

    L_IdcEvnt2Evnt = VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay7_DSTATE;
    if (rtb_RelationalOperator1_nk)
    {
      rtb_f32max = lib_utils_maximum_f32(L_IdcEvnt2Evnt, cVcVmcDsr_L_MinIdcEvnt2Evnt);
      VcVmcDsr_B.VcVmcDsr_ODC_Present.Sum2 = ((1.0F - cVcVmcDsr_Z_FiltCnstIdcDistRt) * (L_IdcDist / rtb_f32max)) + (VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay3_DSTATE * cVcVmcDsr_Z_FiltCnstIdcDistRt);
    }

    if (cVcVmcDsr_B_DsrStatsRst)
    {
      rtb_MultiportSwitch_i = 0.0F;
    }
    else
    {
      rtb_MultiportSwitch_i = VcVmcDsr_B.VcVmcDsr_ODC_Present.Sum2;
    }

    sVcVmcDsr_rt_IdcDist = rtb_MultiportSwitch_i;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE = sVcVmcDsr_D_DrvlnCmd;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE = L_IdcDist;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay5_DSTATE = rtb_B_IdcSelSw;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_n = t_CurrentIdcDurtn;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e = rtb_f32min;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_n = rtb_B_IdcSelSw;
    for (i = 0; i < 10; i++)
    {
      VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[i] = rtb_Switch[i];
      VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[i] = rtb_Switch1[i];
    }

    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay7_DSTATE = rtb_f32min;
    VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay3_DSTATE = rtb_MultiportSwitch_i;
  }
}

void VcVmcDsr_initialize(void)
{
  VcVmcDsr_B.VcVmcDsr_ODC_Present.VariantMergeForOutportxVcVmcDsr = 0;
}

void VcVmcDsr(void)
{
  VcVmcDsr_step();
}

void RESTART_VcVmcDsr(void)
{
  VcVmcDsr_initialize();
}

void updateVariables()
{
  TBD_DrvSituationDetection_IdcInhbt = __VERIFIER_nondet_uchar();
  __VERIFIER_assume(TBD_DrvSituationDetection_IdcInhbt <= 1);
  __VERIFIER_assume(TBD_DrvSituationDetection_IdcInhbt >= 0);
  TBD_DrvSituationDetection_NoIdcReq = __VERIFIER_nondet_uchar();
  __VERIFIER_assume(TBD_DrvSituationDetection_NoIdcReq <= 1);
  __VERIFIER_assume(TBD_DrvSituationDetection_NoIdcReq >= 0);
  TBD_Spare_IdcInhbt = __VERIFIER_nondet_uchar();
  __VERIFIER_assume(TBD_Spare_IdcInhbt <= 1);
  __VERIFIER_assume(TBD_Spare_IdcInhbt >= 0);
  TBD_Spare_NoIdcReq = __VERIFIER_nondet_uchar();
  __VERIFIER_assume(TBD_Spare_NoIdcReq <= 1);
  __VERIFIER_assume(TBD_Spare_NoIdcReq >= 0);
  XXXTrnGearState_No_Qf = __VERIFIER_nondet_uchar();
  __VERIFIER_assume(XXXTrnGearState_No_Qf <= 3);
  __VERIFIER_assume(XXXTrnGearState_No_Qf >= 0);
  XXX_D_Gear = __VERIFIER_nondet_uchar();
  __VERIFIER_assume(XXX_D_Gear <= 11);
  __VERIFIER_assume(XXX_D_Gear >= 0);
  sVcDeScl_D_CcStatus = __VERIFIER_nondet_uchar();
  __VERIFIER_assume(sVcDeScl_D_CcStatus <= 255);
  __VERIFIER_assume(sVcDeScl_D_CcStatus >= 0);
  sVcDeScl_v_CcSet = __VERIFIER_nondet_float();
  __VERIFIER_assume(sVcDeScl_v_CcSet <= 320);
  __VERIFIER_assume(sVcDeScl_v_CcSet >= 0);
  sVcDi_Qf_AccPed = __VERIFIER_nondet_uchar();
  __VERIFIER_assume(sVcDi_Qf_AccPed <= 3);
  __VERIFIER_assume(sVcDi_Qf_AccPed >= 0);
  sVcDi_X_AccPed = __VERIFIER_nondet_float();
  __VERIFIER_assume(sVcDi_X_AccPed <= 100);
  __VERIFIER_assume(sVcDi_X_AccPed >= 0);
  sVcDi_Xd_AccPed = __VERIFIER_nondet_float();
  __VERIFIER_assume(1);
  __VERIFIER_assume(1);
  sVcDtcAtf_Tq_Min = __VERIFIER_nondet_float();
  __VERIFIER_assume(sVcDtcAtf_Tq_Min <= 500);
  __VERIFIER_assume(sVcDtcAtf_Tq_Min >= (- 500));
  sVcEc_v_Veh = __VERIFIER_nondet_float();
  __VERIFIER_assume(sVcEc_v_Veh <= 320);
  __VERIFIER_assume(sVcEc_v_Veh >= 0);
  sVcIfBard_Qf_BpedStat = __VERIFIER_nondet_uchar();
  __VERIFIER_assume(sVcIfBard_Qf_BpedStat <= 3);
  __VERIFIER_assume(sVcIfBard_Qf_BpedStat >= 0);
  sVcIfRge_Qf_RdGrade = __VERIFIER_nondet_uchar();
  __VERIFIER_assume(sVcIfRge_Qf_RdGrade <= 3);
  __VERIFIER_assume(sVcIfRge_Qf_RdGrade >= 0);
  sVcIfRge_X_RdGrade = __VERIFIER_nondet_float();
  __VERIFIER_assume(sVcIfRge_X_RdGrade <= 100);
  __VERIFIER_assume(sVcIfRge_X_RdGrade >= (- 100));
  sVcTc_D_CpedState = __VERIFIER_nondet_uchar();
  __VERIFIER_assume(sVcTc_D_CpedState <= 2);
  __VERIFIER_assume(sVcTc_D_CpedState >= 0);
  sVcTc_D_CpedStateChk = __VERIFIER_nondet_uchar();
  __VERIFIER_assume(sVcTc_D_CpedStateChk <= 3);
  __VERIFIER_assume(sVcTc_D_CpedStateChk >= 0);
  sVcTc_n_GearBoxIn = __VERIFIER_nondet_float();
  __VERIFIER_assume(sVcTc_n_GearBoxIn <= 50000);
  __VERIFIER_assume(sVcTc_n_GearBoxIn >= 0);
  sVcVmcAjc_Tq_Req = __VERIFIER_nondet_float();
  __VERIFIER_assume(sVcVmcAjc_Tq_Req <= 500);
  __VERIFIER_assume(sVcVmcAjc_Tq_Req >= (- 500));
  sVcVmcPmm_D_DrvlnState = __VERIFIER_nondet_uchar();
  __VERIFIER_assume(sVcVmcPmm_D_DrvlnState <= 255);
  __VERIFIER_assume(sVcVmcPmm_D_DrvlnState >= 0);
  sVcVmcPmm_D_PtcIdcError = __VERIFIER_nondet_uchar();
  __VERIFIER_assume(sVcVmcPmm_D_PtcIdcError <= 255);
  __VERIFIER_assume(sVcVmcPmm_D_PtcIdcError >= 0);
  yVcDi_B_IdcSelSw = __VERIFIER_nondet_uchar();
  __VERIFIER_assume(yVcDi_B_IdcSelSw <= 1);
  __VERIFIER_assume(yVcDi_B_IdcSelSw >= 0);
  yVcIfBard_B_BpedStat = __VERIFIER_nondet_uchar();
  __VERIFIER_assume(yVcIfBard_B_BpedStat <= 1);
  __VERIFIER_assume(yVcIfBard_B_BpedStat >= 0);
}

int main(void)
{
  unsigned int counter = 1;
  unsigned int hasOverflown = 0;
  unsigned char sVcVmcDsr_D_IdcState_last_1;
  unsigned char sVcVmcPmm_D_DrvlnState_last_1;
  unsigned char xVcVmcDsr_B_HystStateVehSpdMin_last_1;
  unsigned char xVcVmcDsr_B_HystStateVehSpdMax_last_1;
  unsigned char xVcVmcDsr_B_IdcInhbt_last_1;
  const unsigned int k = 1;
  unsigned int i = 0;
  {
    {
      sVcIfBard_Qf_BpedStat = __VERIFIER_nondet_uchar();
    }
    {
      sVcTc_D_CpedState = __VERIFIER_nondet_uchar();
    }
    {
      sVcVmcDsr_D_IdcState_last_1 = __VERIFIER_nondet_uchar();
    }
    {
      sVcVmcPmm_D_DrvlnState = __VERIFIER_nondet_uchar();
    }
    {
      yVcIfBard_B_BpedStat = __VERIFIER_nondet_uchar();
    }
    {
      TBD_DrvSituationDetection_NoIdcReq = __VERIFIER_nondet_uchar();
    }
    {
      XXXTrnGearState_No_Qf = __VERIFIER_nondet_uchar();
    }
    {
      sVcDi_X_AccPed = __VERIFIER_nondet_float();
    }
    {
      sVcVmcDsr_D_IdcState = __VERIFIER_nondet_uchar();
    }
    {
      TBD_DrvSituationDetection_IdcInhbt = __VERIFIER_nondet_uchar();
    }
    {
      {
        tVcVmcDsr_v_SpdIncIdcTerm_x.xLength = __VERIFIER_nondet_uchar();
      }
      {
        tVcVmcDsr_v_SpdIncIdcTerm_x.xMethod = __VERIFIER_nondet_uchar();
      }
      {
        tVcVmcDsr_v_SpdIncIdcTerm_x.xType = __VERIFIER_nondet_uchar();
      }
      {
        tVcVmcDsr_v_SpdIncIdcTerm_x._space0_ = __VERIFIER_nondet_uchar();
      }
      {
        tVcVmcDsr_v_SpdIncIdcTerm_x.p_xData = __VERIFIER_nondet_pointer();
      }
      {
        {
          tVcVmcDsr_v_SpdIncIdcTerm_x.xCoefs[0] = __VERIFIER_nondet_float();
        }
        {
          tVcVmcDsr_v_SpdIncIdcTerm_x.xCoefs[1] = __VERIFIER_nondet_float();
        }
        {
          tVcVmcDsr_v_SpdIncIdcTerm_x.xCoefs[2] = __VERIFIER_nondet_float();
        }
        {
          tVcVmcDsr_v_SpdIncIdcTerm_x.xCoefs[3] = __VERIFIER_nondet_float();
        }
      }
    }
    {
      sVcVmcPmm_D_PtcIdcError = __VERIFIER_nondet_uchar();
    }
    {
      TBD_Spare_IdcInhbt = __VERIFIER_nondet_uchar();
    }
    {
      sVcDi_Qf_AccPed = __VERIFIER_nondet_uchar();
    }
    {
      {
        {
          VcVmcDsr_B.VcVmcDsr_ODC_Present.Sum2 = __VERIFIER_nondet_float();
        }
        {
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[0] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[1] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[2] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[3] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[4] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[5] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[6] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[7] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[8] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[9] = __VERIFIER_nondet_float();
          }
        }
        {
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[0] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[1] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[2] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[3] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[4] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[5] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[6] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[7] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[8] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[9] = __VERIFIER_nondet_float();
          }
        }
        {
          VcVmcDsr_B.VcVmcDsr_ODC_Present.VariantMergeForOutportxVcVmcDsr = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_B.VcVmcDsr_ODC_Present.xVcVmcDsr_B_IdclInhbtAftrDrvlnC = __VERIFIER_nondet_uchar();
        }
        {
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay2 = __VERIFIER_nondet_uchar();
          }
        }
      }
    }
    {
      XXX_D_Gear = __VERIFIER_nondet_uchar();
    }
    {
      sVcDtcAtf_Tq_Min = __VERIFIER_nondet_float();
    }
    {
      TBD_DrvSituationDetection_IdcInhbt = __VERIFIER_nondet_uchar();
    }
    {
      sVcDi_Xd_AccPed = __VERIFIER_nondet_float();
    }
    {
      {
        tVcVmcDsr_v_SpdIncIdcTerm_x.xLength = __VERIFIER_nondet_uchar();
      }
      {
        tVcVmcDsr_v_SpdIncIdcTerm_x.xMethod = __VERIFIER_nondet_uchar();
      }
      {
        tVcVmcDsr_v_SpdIncIdcTerm_x.xType = __VERIFIER_nondet_uchar();
      }
      {
        tVcVmcDsr_v_SpdIncIdcTerm_x._space0_ = __VERIFIER_nondet_uchar();
      }
      {
        tVcVmcDsr_v_SpdIncIdcTerm_x.p_xData = __VERIFIER_nondet_pointer();
      }
      {
        {
          tVcVmcDsr_v_SpdIncIdcTerm_x.xCoefs[0] = __VERIFIER_nondet_float();
        }
        {
          tVcVmcDsr_v_SpdIncIdcTerm_x.xCoefs[1] = __VERIFIER_nondet_float();
        }
        {
          tVcVmcDsr_v_SpdIncIdcTerm_x.xCoefs[2] = __VERIFIER_nondet_float();
        }
        {
          tVcVmcDsr_v_SpdIncIdcTerm_x.xCoefs[3] = __VERIFIER_nondet_float();
        }
      }
    }
    {
      sVcVmcPmm_D_DrvlnState = __VERIFIER_nondet_uchar();
    }
    {
      sVcTc_D_CpedState = __VERIFIER_nondet_uchar();
    }
    {
      sVcIfBard_Qf_BpedStat = __VERIFIER_nondet_uchar();
    }
    {
      sVcVmcAjc_Tq_Req = __VERIFIER_nondet_float();
    }
    {
      sVcIfRge_X_RdGrade = __VERIFIER_nondet_float();
    }
    {
      sVcEc_v_Veh = __VERIFIER_nondet_float();
    }
    {
      sVcTc_n_GearBoxIn = __VERIFIER_nondet_float();
    }
    {
      sVcDi_Qf_AccPed = __VERIFIER_nondet_uchar();
    }
    {
      xVcVmcDsr_B_IdcInhbt = __VERIFIER_nondet_uchar();
    }
    {
      sVcTc_n_GearBoxIn = __VERIFIER_nondet_float();
    }
    {
      XXXTrnGearState_No_Qf = __VERIFIER_nondet_uchar();
    }
    {
      sVcDeScl_v_CcSet = __VERIFIER_nondet_float();
    }
    {
      sVcDeScl_v_CcSet = __VERIFIER_nondet_float();
    }
    {
      {
        {
          VcVmcDsr_B.VcVmcDsr_ODC_Present.Sum2 = __VERIFIER_nondet_float();
        }
        {
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[0] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[1] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[2] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[3] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[4] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[5] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[6] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[7] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[8] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1[9] = __VERIFIER_nondet_float();
          }
        }
        {
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[0] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[1] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[2] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[3] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[4] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[5] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[6] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[7] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[8] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.Assignment1_d[9] = __VERIFIER_nondet_float();
          }
        }
        {
          VcVmcDsr_B.VcVmcDsr_ODC_Present.VariantMergeForOutportxVcVmcDsr = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_B.VcVmcDsr_ODC_Present.xVcVmcDsr_B_IdclInhbtAftrDrvlnC = __VERIFIER_nondet_uchar();
        }
        {
          {
            VcVmcDsr_B.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay2 = __VERIFIER_nondet_uchar();
          }
        }
      }
    }
    {
      sVcDi_Xd_AccPed = __VERIFIER_nondet_float();
    }
    {
      sVcTc_D_CpedStateChk = __VERIFIER_nondet_uchar();
    }
    {
      sVcDi_X_AccPed = __VERIFIER_nondet_float();
    }
    {
      sVcVmcPmm_D_DrvlnState_last_1 = __VERIFIER_nondet_uchar();
    }
    {
      sVcVmcPmm_D_PtcIdcError = __VERIFIER_nondet_uchar();
    }
    {
      sVcEc_v_Veh = __VERIFIER_nondet_float();
    }
    {
      yVcIfBard_B_BpedStat = __VERIFIER_nondet_uchar();
    }
    {
      counter = __VERIFIER_nondet_uint();
    }
    {
      sVcDtcAtf_Tq_Min = __VERIFIER_nondet_float();
    }
    {
      hasOverflown = __VERIFIER_nondet_uint();
    }
    {
      yVcDi_B_IdcSelSw = __VERIFIER_nondet_uchar();
    }
    {
      yVcDi_B_IdcSelSw = __VERIFIER_nondet_uchar();
    }
    {
      {
        tVcVmcDsr_v_CcDeltaIdcTerm_x.xLength = __VERIFIER_nondet_uchar();
      }
      {
        tVcVmcDsr_v_CcDeltaIdcTerm_x.xMethod = __VERIFIER_nondet_uchar();
      }
      {
        tVcVmcDsr_v_CcDeltaIdcTerm_x.xType = __VERIFIER_nondet_uchar();
      }
      {
        tVcVmcDsr_v_CcDeltaIdcTerm_x._space0_ = __VERIFIER_nondet_uchar();
      }
      {
        tVcVmcDsr_v_CcDeltaIdcTerm_x.p_xData = __VERIFIER_nondet_pointer();
      }
      {
        {
          tVcVmcDsr_v_CcDeltaIdcTerm_x.xCoefs[0] = __VERIFIER_nondet_float();
        }
        {
          tVcVmcDsr_v_CcDeltaIdcTerm_x.xCoefs[1] = __VERIFIER_nondet_float();
        }
        {
          tVcVmcDsr_v_CcDeltaIdcTerm_x.xCoefs[2] = __VERIFIER_nondet_float();
        }
        {
          tVcVmcDsr_v_CcDeltaIdcTerm_x.xCoefs[3] = __VERIFIER_nondet_float();
        }
      }
    }
    {
      sVcVmcAjc_Tq_Req = __VERIFIER_nondet_float();
    }
    {
      TBD_Spare_NoIdcReq = __VERIFIER_nondet_uchar();
    }
    {
      {
        tVcVmcDsr_v_CcDeltaIdcTerm_x.xLength = __VERIFIER_nondet_uchar();
      }
      {
        tVcVmcDsr_v_CcDeltaIdcTerm_x.xMethod = __VERIFIER_nondet_uchar();
      }
      {
        tVcVmcDsr_v_CcDeltaIdcTerm_x.xType = __VERIFIER_nondet_uchar();
      }
      {
        tVcVmcDsr_v_CcDeltaIdcTerm_x._space0_ = __VERIFIER_nondet_uchar();
      }
      {
        tVcVmcDsr_v_CcDeltaIdcTerm_x.p_xData = __VERIFIER_nondet_pointer();
      }
      {
        {
          tVcVmcDsr_v_CcDeltaIdcTerm_x.xCoefs[0] = __VERIFIER_nondet_float();
        }
        {
          tVcVmcDsr_v_CcDeltaIdcTerm_x.xCoefs[1] = __VERIFIER_nondet_float();
        }
        {
          tVcVmcDsr_v_CcDeltaIdcTerm_x.xCoefs[2] = __VERIFIER_nondet_float();
        }
        {
          tVcVmcDsr_v_CcDeltaIdcTerm_x.xCoefs[3] = __VERIFIER_nondet_float();
        }
      }
    }
    {
      {
        VcVmcDsr_DWork.pvc_fast_ts = __VERIFIER_nondet_float();
      }
      {
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE = __VERIFIER_nondet_float();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_n = __VERIFIER_nondet_float();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e = __VERIFIER_nondet_float();
        }
        {
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[0] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[1] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[2] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[3] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[4] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[5] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[6] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[7] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[8] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[9] = __VERIFIER_nondet_float();
          }
        }
        {
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[0] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[1] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[2] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[3] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[4] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[5] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[6] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[7] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[8] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[9] = __VERIFIER_nondet_float();
          }
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay7_DSTATE = __VERIFIER_nondet_float();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay3_DSTATE = __VERIFIER_nondet_float();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE_h = __VERIFIER_nondet_float();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE = __VERIFIER_nondet_float();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE_e = __VERIFIER_nondet_float();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.t_Timer = __VERIFIER_nondet_float();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE_ew = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay5_DSTATE = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_n = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_ne = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay3_DSTATE_o = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_j = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE_o = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_d = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_f = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_fe = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_b = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_e = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_fx = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_bx = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_o = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_fz = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_bh = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE_o4 = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_js = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_active_c15_VcVmcDsr_lib = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_c15_VcVmcDsr_lib = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_NORMAL_OPERATION = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IN_IDC_SPEED_RANGE = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IDC_AVAILABILITY = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_OUT_OF_IDC_SPEED_RANGE = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_active_c10_VcVmcDsr_lib = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_c10_VcVmcDsr_lib = __VERIFIER_nondet_uchar();
        }
        {
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.t_Timer = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_active_c5_VcVmcDsr_lib = __VERIFIER_nondet_uchar();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_c5_VcVmcDsr_lib = __VERIFIER_nondet_uchar();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_TRANSFER_dVcVmcDsr_D_DrvlnCm = __VERIFIER_nondet_uchar();
          }
        }
        {
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3230_MTIDC.is_active_c3_VcVmcDsr_lib = __VERIFIER_nondet_uchar();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3230_MTIDC.is_c3_VcVmcDsr_lib = __VERIFIER_nondet_uchar();
          }
        }
        {
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE = __VERIFIER_nondet_uchar();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay2_DSTATE = __VERIFIER_nondet_uchar();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE_m = __VERIFIER_nondet_uchar();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE_p = __VERIFIER_nondet_uchar();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE_a = __VERIFIER_nondet_uchar();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE_c = __VERIFIER_nondet_uchar();
          }
        }
      }
    }
    {
      sVcDeScl_D_CcStatus = __VERIFIER_nondet_uchar();
    }
    {
      sVcIfRge_Qf_RdGrade = __VERIFIER_nondet_uchar();
    }
    {
      xVcVmcDsr_B_HystStateVehSpdMin = __VERIFIER_nondet_uchar();
    }
    {
      TBD_Spare_IdcInhbt = __VERIFIER_nondet_uchar();
    }
    {
      xVcVmcDsr_B_IdcInhbt_last_1 = __VERIFIER_nondet_uchar();
    }
    {
      xVcVmcDsr_B_HystStateVehSpdMax = __VERIFIER_nondet_uchar();
    }
    {
      XXX_D_Gear = __VERIFIER_nondet_uchar();
    }
    {
      xVcVmcDsr_B_OutOfIdcVehSpdRng = __VERIFIER_nondet_uchar();
    }
    {
      sVcTc_D_CpedStateChk = __VERIFIER_nondet_uchar();
    }
    {
      sVcIfRge_X_RdGrade = __VERIFIER_nondet_float();
    }
    {
      TBD_DrvSituationDetection_NoIdcReq = __VERIFIER_nondet_uchar();
    }
    {
      xVcVmcDsr_B_HystStateVehSpdMin_last_1 = __VERIFIER_nondet_uchar();
    }
    {
      {
        VcVmcDsr_DWork.pvc_fast_ts = __VERIFIER_nondet_float();
      }
      {
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE = __VERIFIER_nondet_float();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_n = __VERIFIER_nondet_float();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e = __VERIFIER_nondet_float();
        }
        {
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[0] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[1] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[2] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[3] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[4] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[5] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[6] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[7] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[8] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay6_DSTATE_e0[9] = __VERIFIER_nondet_float();
          }
        }
        {
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[0] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[1] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[2] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[3] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[4] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[5] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[6] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[7] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[8] = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE[9] = __VERIFIER_nondet_float();
          }
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay7_DSTATE = __VERIFIER_nondet_float();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay3_DSTATE = __VERIFIER_nondet_float();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay2_DSTATE_h = __VERIFIER_nondet_float();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE = __VERIFIER_nondet_float();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE_e = __VERIFIER_nondet_float();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.t_Timer = __VERIFIER_nondet_float();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE_ew = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay5_DSTATE = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_n = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_ne = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay3_DSTATE_o = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_j = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE_o = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_d = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_f = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_fe = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_b = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_e = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_fx = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_bx = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_o = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_fz = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_bh = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay_DSTATE_o4 = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.UnitDelay1_DSTATE_js = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_active_c15_VcVmcDsr_lib = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_c15_VcVmcDsr_lib = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_NORMAL_OPERATION = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IN_IDC_SPEED_RANGE = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_IDC_AVAILABILITY = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_OUT_OF_IDC_SPEED_RANGE = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_active_c10_VcVmcDsr_lib = __VERIFIER_nondet_uchar();
        }
        {
          VcVmcDsr_DWork.VcVmcDsr_ODC_Present.is_c10_VcVmcDsr_lib = __VERIFIER_nondet_uchar();
        }
        {
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.t_Timer = __VERIFIER_nondet_float();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_active_c5_VcVmcDsr_lib = __VERIFIER_nondet_uchar();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_c5_VcVmcDsr_lib = __VERIFIER_nondet_uchar();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x4200_MTIDC.is_TRANSFER_dVcVmcDsr_D_DrvlnCm = __VERIFIER_nondet_uchar();
          }
        }
        {
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3230_MTIDC.is_active_c3_VcVmcDsr_lib = __VERIFIER_nondet_uchar();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3230_MTIDC.is_c3_VcVmcDsr_lib = __VERIFIER_nondet_uchar();
          }
        }
        {
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE = __VERIFIER_nondet_uchar();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay2_DSTATE = __VERIFIER_nondet_uchar();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE_m = __VERIFIER_nondet_uchar();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE_p = __VERIFIER_nondet_uchar();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE_a = __VERIFIER_nondet_uchar();
          }
          {
            VcVmcDsr_DWork.VcVmcDsr_ODC_Present.x3140_MTIDC.UnitDelay1_DSTATE_c = __VERIFIER_nondet_uchar();
          }
        }
      }
    }
    {
      TBD_Spare_NoIdcReq = __VERIFIER_nondet_uchar();
    }
    {
      sVcIfRge_Qf_RdGrade = __VERIFIER_nondet_uchar();
    }
    {
      xVcVmcDsr_B_HystStateVehSpdMax_last_1 = __VERIFIER_nondet_uchar();
    }
    {
      sVcDeScl_D_CcStatus = __VERIFIER_nondet_uchar();
    }
  }
  while (1)
  {
    __VERIFIER_assume(! (! ((! (((sVcVmcDsr_D_IdcState_last_1 == 3) && (! (! (sVcVmcPmm_D_DrvlnState_last_1 == 0)))) && (((! (sVcVmcPmm_D_DrvlnState_last_1 == 0)) && ((xVcVmcDsr_B_HystStateVehSpdMin_last_1 == 0) && (xVcVmcDsr_B_HystStateVehSpdMax_last_1 == 0))) && (xVcVmcDsr_B_IdcInhbt_last_1 >= 1)))) || ((sVcVmcDsr_D_IdcState == 7) && (xVcVmcDsr_B_OutOfIdcVehSpdRng == 0)))));
    i++;
    updateVariables();
    VcVmcDsr();
    if ((counter >= 2) || hasOverflown)
    {
      if ((! ((! (((sVcVmcDsr_D_IdcState_last_1 == 3) && (! (! (sVcVmcPmm_D_DrvlnState_last_1 == 0)))) && (((! (sVcVmcPmm_D_DrvlnState_last_1 == 0)) && ((xVcVmcDsr_B_HystStateVehSpdMin_last_1 == 0) && (xVcVmcDsr_B_HystStateVehSpdMax_last_1 == 0))) && (xVcVmcDsr_B_IdcInhbt_last_1 >= 1)))) || ((sVcVmcDsr_D_IdcState == 7) && (xVcVmcDsr_B_OutOfIdcVehSpdRng == 0)))) && ((i == k)))
      {
        __VERIFIER_error();
      }

    }

    sVcVmcDsr_D_IdcState_last_1 = sVcVmcDsr_D_IdcState;
    sVcVmcPmm_D_DrvlnState_last_1 = sVcVmcPmm_D_DrvlnState;
    xVcVmcDsr_B_HystStateVehSpdMin_last_1 = xVcVmcDsr_B_HystStateVehSpdMin;
    xVcVmcDsr_B_HystStateVehSpdMax_last_1 = xVcVmcDsr_B_HystStateVehSpdMax;
    xVcVmcDsr_B_IdcInhbt_last_1 = xVcVmcDsr_B_IdcInhbt;
    if (counter > 2)
    {
      hasOverflown = 1;
    }
    else
    {
      ++ counter;
    }

  }

  return 0;
}

