Consumer��
#include "ipc.h"
int main(int argc,char *argv[]) {
	int rate = 3;
	int consumerid=atoi(argv[1]);
	buff_h = 101;
	buff_number = 1;
	cget_h = 103;
	cget_number = 1;
	shm_flg = IPC_CREAT | 0644;
	buff_ptr = (char *)set_shm(buff_h,buff_number,shm_flg);
	cget_ptr = (int *)set_shm(cget_h,cget_number,shm_flg);
	prod_h = 201;
	pmtx_h = 202;
	cons_h = 301;
	cmtx_h = 302;
    sem_flg = IPC_CREAT | 0644;
	sem_val = buff_number;
	prod_sem = set_sem(prod_h,sem_val,sem_flg);
	sem_val = 0;
	cons_sem = set_sem(cons_h,sem_val,sem_flg);
	sem_val = 1;
	cmtx_sem = set_sem(cmtx_h,sem_val,sem_flg);
	if(consumerid==0)
		*cget_ptr=0;
	while(1){
		if(buff_ptr[0]-'A'==consumerid){
			down(cons_sem);
			down(cmtx_sem);
			sleep(rate);
			if(buff_ptr[0]=='A'){
				printf("%d The consumer has glue.\nThe consumer gets tobacco and paper\n",getpid());
			}
			if(buff_ptr[0]=='B'){
				printf("%d The consumer has paper.\nThe consumer gets tobacco and glue\n",getpid());
			}
            if(buff_ptr[0]=='C'){
				printf("%d The consumer has tobacco.\nThe consumer gets glue and paper\n",getpid());
			}
			*cget_ptr = (*cget_ptr+1);
			if(*cget_ptr%2==0)
				buff_ptr[0]='D';
			else
				buff_ptr[0]='E';
			up(cmtx_sem);
			up(prod_sem);
		}
	}
	return EXIT_SUCCESS;


}


Producer��
#include "ipc.h"
int main(int argc,char *argv[]){
	int rate=3;
	int producerid=atoi(argv[1]);
	buff_h=101;
	buff_number=1;
	pput_h=102;
	pput_number=1;
	shm_flg=IPC_CREAT|0644;
	buff_ptr = (char *)set_shm(buff_h,buff_number,shm_flg);
    pput_ptr = (int *)set_shm(pput_h,pput_number,shm_flg);
	prod_h = 201;
	pmtx_h = 202;
	cons_h = 301;
	cmtx_h = 302;
	sem_flg = IPC_CREAT|0644;
	sem_val = buff_number;
	prod_sem = set_sem(prod_h,sem_val,sem_flg);
	sem_val = 0;
	cons_sem = set_sem(cons_h,sem_val,sem_flg);
	sem_val = 1;
	pmtx_sem = set_sem(pmtx_h,sem_val,sem_flg);
	if(producerid==0){
		buff_ptr[0]='D';
		*pput_ptr=0;
	}
	while(1){
		if(buff_ptr[0]-'D'==producerid){
			down(prod_sem);
			down(pmtx_sem);
			*pput_ptr = (*pput_ptr+1)%3;
			if(*pput_ptr==0){
				buff_ptr[0] = 'A';
				printf("%d The producer gives tobacco and paper\n",getpid());
			}
			if(*pput_ptr==1){
				buff_ptr[0] = 'B';
				printf("%d The producer gives tobacco and glue\n",getpid());
			}
			if(*pput_ptr==2){
				buff_ptr[0] = 'C';
				printf("%d The producer gives glue and paper\n",getpid());
			}
			sleep(rate);
			up(pmtx_sem);
			up(cons_sem);
		}
	}
	return EXIT_SUCCESS;
}