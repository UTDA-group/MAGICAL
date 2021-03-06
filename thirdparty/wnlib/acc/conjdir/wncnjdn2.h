/****************************************************************************

COPYRIGHT NOTICE:

  The source code in this directory is provided free of
  charge to anyone who wants it.  It is in the public domain
  and therefore may be used by anybody for any purpose.  It
  is provided "AS IS" with no warranty of any kind
  whatsoever.  For further details see the README files in
  the wnlib parent directory.

****************************************************************************/

/****************************************************************************

  "cdn" stands for conjugate-direction noisy

****************************************************************************/
#ifndef wncnjdn2H
#define wncnjdn2H


#include "wnlib.h"



#define WN_CDN_NOT_STARTED                -1



typedef struct wn_cdn_context_type_struct *wn_cdn_context_type;
typedef struct wn_cdn_srchdir_type_struct *wn_cdn_srchdir_type;

struct wn_cdn_context_type_struct
{
  int num_vars;

  double (*pfunction)(double vect[],int sequence_number);
  void (*pline_eval)(wn_cdn_context_type c,
                     double f[],double x[],int n,
                     double *search_direction);

  wn_cdn_srchdir_type *coord_search_direction_array,
                        *search_direction_array;
  int num_search_directions,max_num_search_directions;

  double *xa,*fa;
  int num_line_samples,min_num_line_samples,
      num_line_samples_mem;

  double *current_vect,*func_call_vect,
         *old_vect,*coord_direction;

  double ob;
  int code;

  int num_func_calls,max_num_func_calls;

  bool force_optimize_stop_flag /* = FALSE */;

  wn_memgp current_group;
};

struct wn_cdn_srchdir_type_struct
{
  bool is_coord_direction;
  int coord;
  double *dir_vect;

  double x_min,x_max;

  double x0;  /* optimimum x from last iteration */
  double curvature;  /* curvature from last iteration */
  double x_width,   /* x width from last iteration */
         max_x_width;
  
  double ratio_df1_noise;
};

EXTERN void wn_compute_func_call_vect
(
  wn_cdn_context_type c,
  double *search_direction,
  double x
);

EXTERN void wn_cdn_make_context
(
  wn_cdn_context_type *pc,
  double (*pfunction)(double vect[],int sequence_number),
  int num_vars
);

EXTERN void wn_cdn_free_context
(
  wn_cdn_context_type c
);

EXTERN void wn_cdn_set_solution
(
  wn_cdn_context_type c,
  double vect[]
);

EXTERN void wn_cdn_get_solution
(
  wn_cdn_context_type c,
  int *pcode,
  double *pob,
  double vect[]
);

EXTERN void wn_cdn_set_line_eval
(
  wn_cdn_context_type c,
  void (*pline_eval)(wn_cdn_context_type c,
                     double f[],double x[],int n,
                     double *search_direction)
);

EXTERN void wn_cdn_set_coord_x_min
(
  wn_cdn_context_type c,
  int coord,
  double x_min
);

EXTERN void wn_cdn_set_coord_x_max
(
  wn_cdn_context_type c,
  int coord,
  double x_max
);

EXTERN void wn_cdn_set_coord_x0
(
  wn_cdn_context_type c,
  int coord,
  double x0
);


EXTERN void wn_cdn_set_coord_x_width
(
  wn_cdn_context_type c,
  int coord,
  double x_width
);


EXTERN void wn_cdn_set_coord_max_x_width
(
  wn_cdn_context_type c,
  int coord,
  double max_x_width
);


EXTERN void wn_cdn_force_stop(wn_cdn_context_type c);


EXTERN void wn_cdn_optimize
(
  wn_cdn_context_type c,
  int num_func_calls
);



#endif

