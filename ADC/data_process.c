#define error 0.1    //Ѱ���ֵ��ƫ��
int i = 0; //���ڼ���
//��ȡ��ֵ
float get_max(float *array, int length)
{
    float max = 0.0;
    for(i = 3; i<length; i++)
    {
        if(max < array[i])
        {
            max = array[i];
        }
    }
    return max;
}
//���ط�ֵ��ţ�һ������Χ��
int get_index(float *raw_array,int *deal_array,int length,float max)
{
    int freq_num = 0;
    for(i = 0 ; i<length; i++)
    {
         if( (raw_array[i]>(max-error)) && (raw_array[i] < (max + error)))
         {
             deal_array[freq_num] = i;
             freq_num++;
         }
    }
    return freq_num;
}
//ȥ��ͬһ�������ڵ�����
//ע������鳤���Ƿ�ֵ����ĳ���
int delete_data(int *raw_array,int *deal_array,int length)
{
    int freqend_num = 0;
    for(i = 0; i<length; i++)
    {
        if(raw_array[i+1]-raw_array[i] == 1)
        {

        }
        else
        {
            deal_array[freqend_num] = raw_array[i];
//                    printf("%d\n",ADCEndFreq[freqend_num]);
//                    printf("\n%d\n",freqend_num);
            freqend_num++;
        }
    }
    return freqend_num-1;
}

void float_int(int *array,float a)
{
   int z = a*10000;
   int single = 0;
   for( i = 0; i<5 ; i++)
   {
       single = z%10;
       z = z/10;
       array[4-i] = single;
   }
}

void out_phase(int *array,float a)
{
    int z = a/1;   //��������
    int l = (a-z)*100;
    int single = 0;
    for(i = 0; i < 3 ; i++)  //��������ǰ��λ
    {
        single = z%10;
        z = z/10;
        array[2-i] = single;
    }
    single = 0;
    for(i = 3; i<5; i++)
    {
        single = l%10;
        l = l/10;
        array[7-i] = single;
    }
}

//int  get_recq(int *array; int length)
//{
//    //�����ֵ
//    for(i = 0; i<length; i++)
//    {
//        if(max < array[i])
//        {
//            max = array[i];
//            index = i;
//        }
//    }
//    //���ط�ֵ���
//    for(i = 0 ; i<length; i++)
//    {
//         if( (array[i]>max-error) && (array[i] < (max + error)))
//         {
//                ADCFreq[freq_num] = i;
//                freq_num++;
////                    printf("%d\n",i);
//         }
//    }
//        for(i = 0; i<freq_num; i++)
//        {
//            if(ADCFreq[i+1]-ADCFreq[i] == 1)
//            {
//
//            }
//            else
//            {
//                ADCEndFreq[freqend_num] = ADCFreq[i];
////                    printf("%d\n",ADCEndFreq[freqend_num]);
////                    printf("\n%d\n",freqend_num);
//                freqend_num++;
//            }
//
//        }
//
//}
