#ifndef XIAOC_ANALYSIS_H
#define XIAOC_ANALYSIS_H
//����  �����  20160622  xiaoc@pku.edu.cn
#include "ts.h"
#include "tsfile.h"

struct TS
{
	unsigned char data[188];//TS ���ݰ�
	int count;              //ָʾ��ǰTS��ȡ����λ��
	int left;               //ָʾָʾ��ǰλ�û�ʣ�����λ
};

//TS�İ�ͷռ���ĸ��ֽ�,����ͬ���ֽڡ�PID����ʼλ����Ϣ
struct TsHeader
{
	unsigned char  sync_byte;                   //Ϊ�̶���8 �����ֶΣ���ֵΪ'0100 0111' (0x47)��
	/*Ϊ1 ���ر�־������1 ʱ����ָʾ����ش������������ٴ���1 
	������У�����ش����˱��ؿ����ɴ���������ʵ������Ϊ��1����
	����Ϊ��1��ʱ���˱���Ӧ����������Ϊ��0��������������ֵ
	�Ѿ�У����	**/
	unsigned char  transport_error_indicator;   //�����ʶ
	unsigned char  payload_unit_start_indicator;//��Ч�غɵ�Ԫ��ʼ��:1 ��ʾ�µ�һ֡�Ŀ�ʼ
	/*
	Ϊ1 ����ָʾ��������Ϊ��1��ʱ����ָʾ����ذ��Ⱦ�����ͬPID
	�������иñ�������Ϊ1 ���������и�������ȼ���������ƿ���
	ʹ�ø��ֶ����ȿ��ǻ������ڵĸð����ݡ�ȡ����Ӧ�ã�transport
	_priority �ֶο��Բ���PID���ߴ��ֶν���һ��PID ��Χ�ڱ��롣
	���ֶο������ŵ��ض�����������������ı䡣
	**/
	unsigned char  transport_priority;         //���ȼ�   

	/*PID ֵ0x0000 Ϊ��Ŀ��ر�������  PAT
	  PID ֵ0x0001 Ϊ���������ʱ�������
	  PID ֵ0x0002 Ϊ������������������
	  PID ֵ0x0003 ΪIPMP ������Ϣ������ISO/IEC 13818-11����
	  PID ֵ0x0004-0x000F Ϊ������ֵ��
	  PID ֵ0x0010-0x1FFE ��ָ��Ϊ network_PID, Program_map_PID, elementary_PID, ��Ϊ������Ŀ��
	  PID ֵ0x1FFF Ϊ�հ���������	**/
	unsigned int   PID;                        //PID Ϊ13 �����ֶΣ�ָʾ����Ч�غ��д洢���������͡�
	/*��������ͷ�Լ�����Ӧ�ֶ������ڣ�Ӧ�����š�
	  �ڿհ�������У�transport_scrambling_control �ֶε�ֵӦ����Ϊ��00��
	  00 ������
	  01 �û�����
	  10 �û�����
	  11 �û�����
	**/
	unsigned char  transport_scrambling_control;//��2 �����ֶ�ָʾ����������Ч�غɵļ��ŷ�ʽ
	/* ITU-T H.222.0 ������|ISO/IEC 13818-1 ������Ӧ��������adaptation_field_control �ֶ�����Ϊ
	  ��00��ֵ�Ĵ����������ڿհ�������У�adaptation_field_control �ĸ�ֵӦΪ��01����
	    00 ��δ��ʹ�ã���ISO/IEC ������
	    01 �� adaptation_field������Ч�غ�
	    10 ����Adaptation_field������Ч�غ�
	    11 Adaptation_field ������Ч�غ�
	**/
	unsigned char  adaptation_field_control;   //��2 �����ֶ�ָʾ�˴�������ͷ�Ƿ��������Ӧ�ֶκ�/����Ч�غ�
	/*continuity_counter Ϊ4 �����ֶΣ����ž�����ͬPID ��ÿ���������������ӡ�
	  continuity_counter ��ȡ�����ֵ֮��ѭ�����ص�0 ֵ��������adaptation_field_control Ϊ��00����10��ʱ��
	  continuity_counter �����ӡ�
	  �������У����ư�������Ϊ2 �����ҽ�ֻ��Ϊ2 ��������ͬPID �����������������ø��ư�������
	  ��ԭʼ����ͬ�� continuity_counter ��ֵ���� adaptation_field_control �ֶα�����ڡ�01����11��������
	  ���У�ԭʼ����ÿ���ֽڶ����븴�ƣ���һ��������⣬����Ŀʱ�Ӳο��ֶ��е����������ڣ�����Ч
	  ֵ������롣
	  �ض�����������continuity_counter �������ģ�ֻҪ���������ͬPID ����ǰ�Ĵ��������е�
	  continuity_counter ��ֵ�Ĳ�Ϊ��ֵ1����������һ��������������adaptation_field_control ����Ϊ��00����
	  ��10���������������ĸ��ư���������ʱ��discontinuity_indicator ����Ϊ��1��ʱ�������Լ����������жϣ���
	  ��2.4.3.4�����ڿհ�������У�continuity_counter ֵδȷ����
	**/
	unsigned char  continuity_counter;         //���ž�����ͬPID ��ÿ���������������ӡ�
};

struct TSPAT
{
	/*table_id �ֶα�ʶ������PSI �ֶε�����
	0x00 program_association_section��PAT��
	0x01 conditional_access_section (CA_section)
	0x02 TS_program_map_section PMT
	0x03 TS_description_section
	0x04 ISO_IEC_14496_scene_description_section
	0x05 ISO_IEC_14496_object_descriptor_section
	0x06 Metadata_section
	0x07 IPMP_Control_Information_section (defined in ISO/IEC 13818-11)
	0x08-0x3F ITU-T H.222.0 ������ | ISO/IEC 13818-1 ����
	0x40-0xFE �û�ר��
	0xFF ���� **/
	unsigned char  table_id;                //��Ϊ8 �����ֶΣ�Ӧ����Ϊ0x00
	unsigned char  section_syntax_indicator;//section_syntax_indicator Ϊ1 �����ֶΣ�Ӧ����Ϊ��1����
	unsigned char  zero0;                   //�̶�ֵ0
	unsigned char  reserved0;               //�����ֶ�
	/*��Ϊ12 �����ֶΣ����ֶε�ͷ�����ر�Ϊ��00����ʣ��10 ����ָ���÷ֶε��ֽ�����
	  ����ֶγ����ֶο�ʼ��������CRC�����ֶ��У���ֵӦ������1021��0x3FD����**/
	unsigned int   section_length;          //����
	unsigned int   transport_stream_id;     //��Ϊ16 �����ֶΣ����ֶγ䵱��ǩ����ʶ�����ڴ˴������б����κ�������·����������ֵ���û��涨��
	unsigned char  reserved1;               //�����ֶ�
	/*��5 �����ֶ�Ϊ������Ŀ��ر��İ汾�š�����Ŀ��ر��Ķ���ı�ʱ���汾��
	Ӧ��1 ģ32��current_next_indicator ����Ϊ��1��ʱ��version_number ����Ϊ��ǰ��Ч�Ľ�Ŀ��ر��İ汾�š�
	current_next_indicator ����Ϊ��0��ʱ��version_number ����Ϊ��һ����Ч�Ľ�Ŀ��ر��İ汾�š�	**/
	unsigned char  version_number;          //�汾��
	/*1 ����ָʾ�������ڡ�1��ʱָʾ���͵Ľ�Ŀ��ر�Ϊ��ǰ��Ч�ġ��ñ���
	����Ϊ��0��ʱ����ָʾ���͵ĸñ���δ��Ч������һ��������Ч	**/
	unsigned char  current_next_indicator;  //��Чλ
	/*��8 �����ֶθ����˷ֶεı�š���Ŀ��ر����׷ֶε�section_number 
	����Ϊ	0x00�����Ž�Ŀ��ر���ÿ�����ӵķֶ���Ӧ��1��**/
	unsigned char  section_number;          //�ֶκ�
	/*��8�����ֶ�ָ��������Ŀ��ر������ֶα�ţ����������section_number
	�ķֶΣ���	**/
	unsigned char  last_section_number;     //���ֶκ�
	unsigned char  N;                       //������Ŀ����
	/* Program_number Ϊ16 �����ֶΡ���ָ��program_map_PID �����õĽ�Ŀ������
	0x0000 ʱ�������PID �ο���Ϊ����PID����������������������ֶε�ֵ���û��涨��
	�ڽ�Ŀ��ر���	һ���汾�ڣ����ֶ�Ӧ��ֻһ�ε�ȡ�κηǵ�һֵ��
	ע �� ���磬���԰� program_number ����Ϊ�㲥�ŵ��ı�ʶ��**/
	unsigned int  program_number[12];      //��Ŀ��
	unsigned char reserved[12];            //�����ֶ�
	/*network_PID Ϊ13 �����ֶΣ���ͬ����Ϊ0x0000 ֵ�� program_number 
	һ��ʹ�ã�	ָ��Ӧ����������Ϣ���Ĵ���������PID��network_PID �ֶε�ֵ���û��Զ��壬
	��������ȡ������ָ����ֵ��network_PID �Ĵ�������ѡ�ġ�	**/
	unsigned int  network_PID[12];             //����������Ϣ���Ĵ���������PID
	/*program_map_PID Ϊ13 �����ֶΣ�������program_number ��ָ���Ľ�Ŀ���ԣ�
	ָ��Ӧ����program_map_section Ӧ�õĴ���������PID ���κ�program_number ��Ӧû�ж��
	program_map_PID ��ֵ��program_map_PID ��ֵ���û����壬��������ȡ��2-3 ����ָ����ֵ��
	**/
	unsigned int  program_map_PID[12];         //PMT
	/*��Ϊ32 �����ֶΣ���������ȫ����Ŀ��طֶκ�**/
	unsigned int  CRC_32;                      //CRC
};

struct TSPMT
{
	/*table_id �ֶα�ʶ������PSI �ֶε�����
	0x00 program_association_section��PAT��
	0x01 conditional_access_section (CA_section)
	0x02 TS_program_map_section PMT
	0x03 TS_description_section
	0x04 ISO_IEC_14496_scene_description_section
	0x05 ISO_IEC_14496_object_descriptor_section
	0x06 Metadata_section
	0x07 IPMP_Control_Information_section (defined in ISO/IEC 13818-11)
	0x08-0x3F ITU-T H.222.0 ������ | ISO/IEC 13818-1 ����
	0x40-0xFE �û�ר��
	0xFF ���� **/
	unsigned char  table_id;                //��Ϊ8 �����ֶΣ�Ӧ����Ϊ0x02
	unsigned char  section_syntax_indicator;//section_syntax_indicator Ϊ1 �����ֶΣ�Ӧ����Ϊ��1����
	unsigned char  zero0;                   //�̶�ֵ0
	unsigned char  reserved0;               //�����ֶ�
	/*��Ϊ12 �����ֶΣ����ֶε�ͷ�����ر�Ϊ��00����ʣ��10 ����ָ���÷ֶε��ֽ�����
	  ����ֶγ����ֶο�ʼ��������CRC�����ֶ��У���ֵӦ������1021��0x3FD����**/
	unsigned int   section_length;          //����
    /* Program_number Ϊ16 �����ֶΡ���ָ��program_map_PID �����õĽ�Ŀ������
	0x0000 ʱ�������PID �ο���Ϊ����PID����������������������ֶε�ֵ���û��涨��
	�ڽ�Ŀ��ر���	һ���汾�ڣ����ֶ�Ӧ��ֻһ�ε�ȡ�κηǵ�һֵ��
	ע �� ���磬���԰� program_number ����Ϊ�㲥�ŵ��ı�ʶ��**/
	unsigned int  program_number;          //��Ŀ��
    /*��5 �����ֶ�Ϊ������Ŀ��ر��İ汾�š�����Ŀ��ر��Ķ���ı�ʱ���汾��
	Ӧ��1 ģ32��current_next_indicator ����Ϊ��1��ʱ��version_number ����Ϊ��ǰ��Ч�Ľ�Ŀ��ر��İ汾�š�
	current_next_indicator ����Ϊ��0��ʱ��version_number ����Ϊ��һ����Ч�Ľ�Ŀ��ر��İ汾�š�	**/
	unsigned char  reserved1;               //�����ֶ�
	unsigned char  version_number;          //�汾��
	/*1 ����ָʾ�������ڡ�1��ʱָʾ���͵Ľ�Ŀ��ر�Ϊ��ǰ��Ч�ġ��ñ���
	����Ϊ��0��ʱ����ָʾ���͵ĸñ���δ��Ч������һ��������Ч	**/
	unsigned char  current_next_indicator;  //��Чλ
	unsigned char  section_number;          //��8 �����ֶ�ֵ��Ϊ0x00��
	unsigned char  last_section_number;     //��8 �����ֶ�ֵ��Ϊ0x00��
	unsigned char  reserved2;               //�����ֶ�
	/*��Ϊ13 �����ֶΣ�ָʾ����������PID���ô�������Ӧ��������program_number ��ָ
	����Ŀ��Ч��PCR �ֶΡ����κ�PCR ����ר�����Ľ�Ŀ�����޹أ�����ֶ�Ӧȡ0x1FFF ֵ
	**/
	unsigned int   PCR_PID;                 //PCR
	unsigned char  reserved3;               //�����ֶ�
	/*��Ϊ12 �����ֶΣ����ֶε�ͷ�����ر�Ϊ��00����ʣ��10 ����ָ������
	program_info_length �ֶε����������ֽ�����**/
	unsigned int   program_info_length;     //�����ֽ���Ŀ
	unsigned char  N;                       //�������ݸ���
	unsigned char  N1;                      //������Ƶ������Ƶ������
	/*��Ϊ8�����ֶΣ�ָʾ����PIDֵ�İ��ڳ��صĽ�ĿԪ���ͣ���PIDֵ��elementary_PID
	��ָ����stream_type ֵ�ڱ���ָ����
	0x00 ITU-T | ISO/IEC ����
	0x01 ISO/IEC 11172-2 ��Ƶ
	0x02 ITU-T H.262 ������ | ISO/IEC 13818-2 ��Ƶ�� ISO/IEC 11172-2 ���޲�����Ƶ��
	0x03 ISO/IEC 11172-3 ��Ƶ
	0x04 ISO/IEC 13818-3 ��Ƶ
	0x05 ITU-T H.222.0 ������ | ISO/IEC 13818-1 private_sections
	0x06 ITU-T H.222.0 ������ | ISO/IEC 13818-1 PES ����ר�����ݵ�PES ��
	0x07 ISO/IEC 13522 MHEG
	0x08 ITU-T H.222.0 ������ | ISO/IEC 13818-1 ����A DSM-CC
	0x09 ITU-T H.222.1 ������
	0x0A ISO/IEC 13818-6 ���� A
	0x0B ISO/IEC 13818-6 ���� B
	0x0C ISO/IEC 13818-6 ���� C
	0x0D ISO/IEC 13818-6 ���� D
	0x0E ITU-T H.222.0 ������ | ISO/IEC 13818-1 ����
	0x0F ����ADTS ����䷨��ISO/IEC 13818-7 ��Ƶ
	0x10 ISO/IEC 14496-2 ����
	0x11 ����LATM ����䷨��ISO/IEC 14496-3 ��Ƶ����ISO/IEC 14496-3 ���涨
	0x12 PES ���г��ص�ISO/IEC 14496-1 SL ��ʽ����FlexMux ��
	0x13 ISO/IEC 14496_ sections �г��ص�ISO/IEC 14496-1 SL ��ʽ����FlexMux ��
	0x14 ISO/IEC 13818-6 ͬ������Э��
	0x15 PES ���г��ص�Ԫ����
	0x16 metadata_sections �г��ص�Ԫ����
	0x17 ISO/IEC 13818-6 ����ת��ʽ�Ŵ��г��ص�Ԫ����
	0x18 ISO/IEC 13818-6 Ŀ��ת��ʽ�Ŵ��г��ص�Ԫ����
	0x19 ISO/IEC 13818-6 ͬ������Э���г��ص�Ԫ����
	0x1A IPMP �� ��ISO/IEC 13818-11, MPEG-2 IPMP �й涨��
	0x1B ITU-T H.264 ������ | ISO/IEC 14496-10 ��Ƶ�ж����AVC ��Ƶ��
	0x24 ITU-T H.265 ������
	0x1C-0x7E ITU-T H.222.0 ������ | ISO/IEC 13818-1 ����
	0x7F IPMP ��
	0x80-0xFF �û�ר��
	**/
	unsigned char  stream_type[12];         //������
	unsigned char  reservedN1_0[12];        //�����ֶ�
	unsigned int   elementary_PID[12];      //��Ϊ13 �����ֶΣ�ָ��������ؽ�ĿԪ�Ĵ���������PID��
	unsigned char  reservedN1_1[12];        //�����ֶ�
	/*��Ϊ12�����ֶΣ����ֶε�ͷ�����ر�Ϊ��00����ʣ��10����ָʾ����ES_info_length
	�ֶε���ؽ�ĿԪ���������ֽ���**/
	unsigned int   ES_info_length[12];      //ES����
	/*��Ϊ32 �����ֶΣ���������ȫ����Ŀ��طֶκ�**/
	unsigned int  CRC_32;                      //CRC
};

/* ����Ӧ�ֶ�adaptation_field*/
struct TSADT
{
	/*adaptation_field_length Ϊָ������adaptation_field_length ��adaptation_field
	�е��ֽ�����8 �����ֶΡ�0 ֵ��ʾ���������в��뵥������ֽڡ���adaptation_field_
	control ֵΪ��11��ʱ��adaptation_field_length ֵ������0 ��182 �������ڡ���adapt
	ation_field_control ֵΪ��10��ʱ��adaptation_field_length ֵ����Ϊ183�����ڳ���
	PES ���Ĵ���������ֻҪ����Ƿ�����PES �����ݾ���Ҫ	ͨ���������ȫ��������������
	��Ч�غ��ֽڡ����ͨ���涨����Ӧ�ֶγ��ȱ�����Ӧ�ֶ�������Ԫ�ĳ����ܺͻ�Ҫ����ʵ
	�֣�����������Ӧ�ֶ�����ȫ������Ч��PES �����ݺ���Ч�غ��ֽ�����ʣ�ࡣ����
	Ӧ�ֶ��ж���ռ��������ֽ�����	**/
	unsigned char adaptation_field_length;//����Ӧ�ֶγ���

	/*��Ϊ1 �����ֶΣ����ڡ�1��ʱ��ָʾ��ǰ���������Ĳ�������״̬Ϊ�档��discontinuity
	_indicator ����Ϊ��0���򲻴���ʱ����������״̬Ϊ�١���������ָʾ������ָʾ��������
	�Ĳ������ԣ�ϵͳʱ����������Ժ�continuity_counter �������ԡ�
	**/
	unsigned char discontinuity_indicator;//��������״̬

	/*random_access_indicator Ϊ1 �����ֶΣ�ָʾ��ǰ�Ĵ��������Լ����ܵ�
	������ͬPID �ĺ��������������ڴ˵������������������ĳЩ��Ϣ��
	�ر�ģ��ñ������ڡ�1��ʱ���ھ��е�ǰPID �Ĵ�����������Ч�غ�����ʼ����һ��PES ���������
	һ��discontinuity_indicator �ֶ��й涨�Ļ���������㡣���⣬����Ƶ����У���ʾʱ���Ǳ�
	���ڸ���	�����������ĵ�һͼ���д��ڡ�
	��Ƶ֡�����ֽڣ�ֻҪPES ������Ϊ3 ��4�����⣬����Ƶ����У���ʾʱ���Ǳ����ڰ�������ͷ
	�ĵ�һͼ���PES ���д��ڡ���PCR_PID ��random_access_indicator �ڰ���PCR �ֶεĴ��������н�����
	��Ϊ��1��**/
	unsigned char random_access_indicator;

	/*elementary_stream_priority_indicator Ϊ1 �����ֶΡ��ھ���	��ͬPID �İ�֮�䣬��ָʾ�˴�����
	����Ч�غ��ڳ��صĻ��������ݵ����ȼ�����1��ָʾ����Ч�غɾ���	����������������Ч�غɸ��ߵ�
	���ȼ���
	��ISO/IEC 11172-2 �� ITU-T H.262 ������ | ISO/IEC 13818-2 �� ISO/IEC 14496-2 ��Ƶ����У����ֶ�
	������Ϊ��1������������Ч�غɰ���һ�����������ڱ��������ֽڡ�
	��ITU-T H.264 ������ | ISO/IEC 14496-10 ��Ƶ����У����ֶο�����Ϊ��1������������Ч�غɰ�����
	��slice_type ��Ϊ2, 4, 7 �� 9 �Ľ����һ�������ֽڡ�
	��0��ֵָʾ����Ч�غɾ���ͬ���в��߱��˱������ڡ�1����������һ�������ȼ���
	**/
	unsigned char elementary_stream_priority_indicator;

	/*PCR_flag Ϊ1 ���ر�־����1��ֵָʾadaptation_field �����������ֱ����PCR �ֶΡ�
	��0��ֵָʾ����Ӧ�ֶβ������κ�PCR �ֶΡ�
	**/
	unsigned char PCR_flag;

	/*OPCR_flag ��־Ϊ1 ���ر�־����1��ֵָʾadaptation_field �����������ֱ����OPCR
	�ֶΡ���0��ֵָʾ����Ӧ�ֶβ������κ�OPCR �ֶΡ�**/
	unsigned char OPCR_flag;

	/*splicing_point_flag Ϊ1 ���ر�־�����ڡ�1��ʱ����ָʾsplice_countdown �ֶ�
	�������������Ӧ�ֶ��д��ڣ�ָ��ƴ�ӵ�ĳ��֡���0��ֵָʾ����Ӧ�ֶ���splice_countdown 
	�ֶβ����ڡ�	**/
	unsigned char splicing_point_flag;

	/*transport_private_data_flag Ϊ1 ���ر�־����1��ֵָʾ����Ӧ�ֶΰ�
	��һ������private_data �ֽڡ���0��ֵָʾ����Ӧ�ֶβ������κ�private_data �ֽڡ�
	**/
	unsigned char transport_private_data_flag;

	/*adaptation_field_extension_flag Ϊ1 �����ֶΣ����ڡ�1��ʱ��ָʾ
	����Ӧ�ֶ���չ�Ĵ��ڡ���0��ֵָʾ����Ӧ�ֶ�������Ӧ�ֶ���չ�����ڡ�
	**/
	unsigned char adaptation_field_extension_flag;

	/*program_clock_reference
	��PCR�� Ϊ�������ֱ����42 �����ֶΡ���һ���֣�program_clock_reference_base Ϊ33 �����ֶΣ���ֵ��
	PCR_ base (i)�������繫ʽ2-2 �и����ġ��ڶ����֣�program_clock_reference_extension Ϊ9 �����ֶΣ���
	ֵ��PCR_ext(i) �������繫ʽ2-3 �и����ġ�PCR ָʾ����program_clock_reference_base �����ص��ֽ�
	����ϵͳĿ�����������˵�Ԥ��ʱ�䡣
	**/
	unsigned int  program_clock_reference_base;
	unsigned char reverse0;//�����ֶ�
	unsigned int  program_clock_reference_extension;

	unsigned int  original_program_clock_reference_base;
	unsigned char reverse1;//�����ֶ�
	unsigned int  riginal_program_clock_reference_extension;

	/*splice_countdown Ϊ8 �����ֶΣ���ʾ����Ϊ���򸺵�ֵ����ֵָʾ��ͬPID ��
	������ش�������ֱ������ƴ�ӵ�Ĵ���������ʣ���������ƵĴ��������ͽ���������Ӧ�ֶεĴ�������
	���ų������splice_countdown �ֶδﵽ��Ĵ��������е�����ֽ�֮���λ�ü�Ϊƴ�ӵ�Ķ�λ����
	splice_countdown �ﵽ��Ĵ��������У�����������Ч�غɵ���������ֽڱ����Ǳ�����Ƶ֡�����ͼ���
	����ֽڡ�����Ƶ����У���Ӧ�Ĵ�ȡ��Ԫ������ sequence_end_code ��ֹ���߲��� sequence_end_code ��
	ֹ���������ġ�������ͬPID �Ĵ����������԰�������ͬһ���͵Ĳ�ͬ�����������ݡ�
	��ͬPID ����һ��������������Ч�غɣ����ư��Լ�����Ч�غɵİ����⣩Ӧ����PES �������ֽڿ�
	ʼ����������Ƶ����У�PES ����Ч�غ�Ӧ�ӽ���㿪ʼ����������Ƶ����У�PES ����Ч�غ�Ӧ�ӽ���
	�㿪ʼ���������߰����������� sequence_end_code ��ʼ��������������ǰ�������Ƶ֡������ͼ��
	������߽綨λ������ͨ������ʵ�֡�ƴ�ӵ�֮�󣬵����ֶ�Ҳ���Դ��ڡ���splice_countdown �ֶ�Ϊ����
	����ֵΪ��n(��n)ʱ����ָʾ��صĴ��������Ǹ���ƴ�ӵ�ĵ�n ���������ư�������Ч�غɵİ����⣩��
	**/
	unsigned char splice_countdown;

	//������	
};

class Anlysis
{
	ts_param*     m_param;         //���ò�����Ϣ

	TS            m_ts;            //TS����
	TsHeader      m_tsHeader;      //TS���ݰ�ͷ
	TSPAT         m_pat;           //PAT����
	TSPMT         m_pmt;           //PMT����
	TSADT         m_adt;           //����Ӧ�ֶ�����

	TsHeader      m_tsNewHeader;   //TS���ݰ�ͷ
	TS            m_newTs;         //������TS����

	unsigned int  m_AudioPID;      //ָʾ��Ƶ���ڵ�PID ��ʼ��Ϊ0
	unsigned int  m_VideoPID;      //ָʾ��Ƶ���ڵ�PID ��ʼ��Ϊ0
	unsigned int  m_PATPID;        //ָʾPAT���ڵ�PID ��ʼ��Ϊ0
	unsigned int  m_PMTPID;        //ָʾPMT���ڵ�PID ��ʼ��Ϊ0

	/*pointer_field �� ��Ϊ8 �����ֶΣ���ֵΪ�ֽ�����������pointer_field ֱ����������
	��Ч�غ��д��ڵ��׷ֶε����ֽڳ���Ϊֹ���ֽ��������pointer_field �е�0x00 ֵָʾ
	pointer_field ��÷ֶ�������ʼ����	������һ���ֶ��ڸ����Ĵ��������п�ʼ����ʱ��
	payload_unit_start_indicator������2.4.3.2��Ӧ���ڡ�1����	�����Ǹ�������������Ч
	�غɵ����ֽڱض�������ָ�롣�������Ĵ���������û���κηֶο�ʼ����ʱ��	payload_unit
	_start_indicator Ӧ���ڡ�0���������Ǹ�������Ч�غ���Ӧ�������κ�ָ�롣	**/
	unsigned char pointer_field;   //ָ��

	int           m_tsConter;      //ͳ��ts�ĸ���    

public:
	Anlysis();
	void create(ts_param *param);/* �����ڴ� */
	void destory();/* �ͷ��ڴ� */
	
	/* ����ts Header�ļ� */
	bool anlysisTsHeader();

	/* ����ts PAT�ļ� */
	void anlysisPAT();

	/* ����ts PMT�ļ� */
	void anlysisPMT();

	/* ����ts ����Ӧ�ֶ� */
	void adaptation_field();

	/* ����ts�ļ� */
	void anlysis();
};
#endif