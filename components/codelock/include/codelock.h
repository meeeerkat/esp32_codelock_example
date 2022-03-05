#ifndef _CODELOCK_H_
#define _CODELOCK_H_

extern void init_codelock(void);
void codelock_set_on_success_callback(void (*callback) (void));
void codelock_set_on_failure_callback(void (*callback) (void));

#endif
