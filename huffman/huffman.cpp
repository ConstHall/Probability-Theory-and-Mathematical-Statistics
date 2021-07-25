#include<iostream>
#include<string>
#include<cstring>
using namespace std; 
struct head
{
    int b;						  //�ַ�
    long count;                   //�ļ��и��ַ����ֵĴ���
    long parent, lch, rch;        //make a tree
    char bits[256];               //the huffuman code
};
 
struct head header[512], tmp;  //�ڵ���
 
//������compress()
//���ã���ȡ�ļ����ݲ�����ѹ��
//��ѹ������д����һ���ĵ�
int compress(const char *filename,const char *outputfile)
{
    char buf[512];
    unsigned char c;
    long i, j, m, n, f;
    long min1, pt1, flength;
    FILE *ifp, *ofp;//ifp����ָ��    ofp��дָ�� 
	int per = 10;
    ifp = fopen(filename, "rb");                  //�Զ����Ʒ�ʽ��ԭʼ�ļ�����ֻ��������� 
    if (ifp == NULL){
        cout << "���ļ�ʧ��:" << filename << endl;
		//printf("���ļ�ʧ��:%s\n",filename);
        return 0;                             //�����ʧ�ܣ������������Ϣ
    } 
    ofp = fopen(outputfile,"wb");                 //��ѹ����洢��Ϣ���ļ�����ֻ����д���� 
    if (ofp == NULL){
        cout << "���ļ�ʧ��:" << outputfile << endl;
        return 0;
    }
    flength = 0;
    while (!feof(ifp)){ //�����������������ļ��Ƿ��������������򷵻ط���ֵ�����򷵻�0 
        fread(&c, 1, 1, ifp); // fread(buffer,size,count,fp); 
		//��1��buffer����һ��ָ�룬��fread��˵�����Ƕ������ݵĴ�ŵ�ַ����fwrite��˵����Ҫ������ݵĵ�ַ��
		//��2��size��Ҫ��д���ֽ�����
		//��3��count:Ҫ���ж�д���ٸ�size�ֽڵ������
		//��4��fp:�ļ���ָ�롣
        header[c].count ++;                       //���ļ���ͳ���ַ����ִ���
        flength ++;                               //��¼�ļ����ַ�����
    }
    flength --;
    header[c].count --;
	// feof������ 
    for (i = 0; i < 512; i ++){                   //HUFFMAN�㷨�г�ʼ�ڵ������
        if (header[i].count != 0){ //������ַ���������Ϊ0 
        	header[i].b = (unsigned char) i; //�򽫽ṹ���е��ַ�b��ֵΪ��ĸi 
		} 
        else{
        	header[i].b = -1; //û���־͸�ֵΪ-1 
		}
        header[i].parent = -1;//���׳�ֵΪ-1 
        header[i].lch = header[i].rch = -1; //����������ֵҲΪ-1 
    }
 
    for (i = 0; i < 256; i ++){                    //���ڵ㰴���ִ�������(ð������)������ 
        for (j = i + 1; j < 256; j ++){
            if (header[i].count < header[j].count){ 
				tmp = header[i];
                header[i] = header[j];
                header[j] = tmp;
            }
        }
    }
 
 
    for (i = 0; i < 256; i ++){                    //ǰ���Ѿ��ѳ��ֵ��ַ��������ź��ˣ�����������Ҫ�ҳ�����õ���Щ�ַ������һ���ַ����ڵ�λ�� 
    //��Ϊ����֮��ʵ������ 5 4 3 2 1 0 0 0 ...0���������У�0֮ǰ�Ķ��Ǹ����ַ��ĳ��ִ��� 
    //Ҳ����ͳ�Ƴ��ֵ��ַ����� 
        if (header[i].count == 0){
        	break;
		} 
	}
 
    n = i;
    //nΪ���ֵ��ַ����� 
    m = 2 * n - 1;
    //mΪhuffman���Ľڵ��� 
    for (i = n; i < m; i ++){ //����huffman�����ܹ�n���ڵ㣬�������е�λ��Ϊn��2n-1��������n���� ���ô�i=n��ʼȫ�ǿսڵ㣨ȨֵΪ0�� 
        min1 = 999999999;
        for (j = 0; j < i; j ++){//����С�ڵ� 
            if (header[j].parent != -1){//�ҳ���δ����huffman������û�и��׽ڵ㣩�Ľڵ�
            	continue;
			}
            if (min1 > header[j].count){
                pt1 = j;
                min1 = header[j].count;
                //�ҳ���ǰȨֵ��С�Ľڵ㣬��������Ȩֵ
				//pt1����ڵ�λ�ã�min1����Ȩֵ 
                continue;
            }
        }
        header[i].count = header[pt1].count;//��ǰ�սڵ��Ȩֵ��Ϊ��С�ڵ��Ȩֵ 
        header[pt1].parent = i;//��С�ڵ�ĸ��׽ڵ�Ϊi���൱��tn 
        header[i].lch = pt1;//������Ϊ��С�ڵ� 
        min1 = 999999999;
        for (j = 0; j < i; j ++){//�ҵڶ�С�ڵ� 
            if (header[j].parent != -1){
            	continue;
			} 
            if (min1 > header[j].count){
                pt1 = j;
                min1 = header[j].count;
                continue;
            }
        }
        header[i].count += header[pt1].count;//��ǰ�ڵ�Ȩֵ��Ϊ��С�ڵ�ʹ�С�ڵ��Ȩֵ�� 
        header[i].rch = pt1;//������Ϊ�ڶ�С�ڵ� 
        header[pt1].parent = i;//�ڶ�С�ڵ�ĸ��׽ڵ�ҲΪi 
    } 
 
    for (i = 0; i < n; i ++){                        //����huffman���������ַ��ı���
        f = i;//iֻ���ڼ�������f������i�Ĳ��� 
        header[i].bits[0] = 0;
        while (header[f].parent != -1){//����ǰ�ڵ��޸��׽ڵ� 
            j = f;//ͬ��j������f 
            f = header[f].parent; 
            if (header[f].lch == j){//�ж�j������������������
			//header[f].lch = header[header[f].parent].lch
                j = strlen(header[i].bits);
                memmove(header[i].bits + 1, header[i].bits, j + 1);
                //memmove���ڿ����ֽڣ����Ŀ�������Դ�������ص��Ļ���memmove�ܹ���֤Դ���ڱ�����֮ǰ���ص�������ֽڿ�����Ŀ�������У�
				//�����ƺ�Դ���ݻᱻ���ġ����ǵ�Ŀ��������Դ����û���ص����memcpy����������ͬ��
				/*void *memmove( void* dest, const void* src, size_t count );
				��src��ָ�ڴ�������count���ֽڵ�dest��ָ�ڴ�����*/ 
                header[i].bits[0] = '0';//�����ڵݹ� 
            }
            else{
                j = strlen(header[i].bits);
                memmove(header[i].bits + 1, header[i].bits, j + 1);
                header[i].bits[0] = '1';
            }
        }
    }
 
    //����ľ��Ƕ�ԭ�ļ���ÿһ���ַ����������úõı����滻�ļ��е��ַ�
    fseek(ifp, 0, SEEK_SET);                                                //��ָ�붨���ļ���ʼλ��
    //int fseek(FILE *stream, long int offset, int whence) ������ stream ���ļ�λ��Ϊ������ƫ�� offset������ offset
	//��ζ�ŴӸ����� whence λ�ò��ҵ��ֽ����� 
    fseek(ofp, 8, SEEK_SET);                                //��8λ��������Ϊ��λ���ж�ȡ
    buf[0] = 0;
    f = 0;
    pt1 = 8;
 	cout << "��ȡ��Ҫѹ�����ļ�:" << filename << endl;
 	cout << "��ǰ�ļ���:" << flength << "�ֽ�" << endl;
 
    while (!feof(ifp)){
        c = fgetc(ifp); // ÿ�ζ�ȡһ���ַ� 
        f ++;
        for (i = 0; i < n; i ++){//��ʼ����ƥ�� 
            if (c == header[i].b) break;
        }
        strcat(buf, header[i].bits);//buf�д�ȡ��ǰ�����ַ��ı���
        j = strlen(buf);//jΪ���볤�� 
        c = 0;
        while (j >= 8){                                             //��ʣ���ַ�������С��8��ʱ
            for (i = 0; i < 8; i ++){							  //���հ�λ��������ת����ʮ����ASCII��д���ļ�һ�ν���ѹ��
                if (buf[i] == '1'){
                	c = (c << 1) | 1;
				}
                else{
                	c = c << 1;
				}
            }
            fwrite(&c, 1, 1, ofp);// size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
			/*ptr-- ����ָ��Ҫ��д���Ԫ�������ָ�롣
			size-- ����Ҫ��д���ÿ��Ԫ�صĴ�С�����ֽ�Ϊ��λ��
			nmemb-- ����Ԫ�صĸ�����ÿ��Ԫ�صĴ�СΪ size �ֽڡ�
			stream-- ����ָ�� FILE �����ָ�룬�� FILE ����ָ����һ���������*/ 
            pt1 ++;
            strcpy(buf, buf + 8);//�൱��bufָ������8λ�����ڴ洢��һ�������ַ���8λ���� 
            j = strlen(buf);
        }
        if (f == flength){//�ļ��������ַ���������� 
        	break;
		}
    }
 
    if (j > 0){                                                     //��ʣ���ַ���������8��ʱ
        strcat(buf, "00000000");									//�Բ����λ�����в���
        for (i = 0; i < 8; i ++){
            if (buf[i] == '1'){
            	c = (c << 1) | 1;
			} 
            else{
				c = c << 1;
			}
        }
        fwrite(&c, 1, 1, ofp);
        pt1 ++;
    }
    fseek(ofp, 0, SEEK_SET);                                        //��������Ϣд��洢�ļ�
	fwrite(&flength,1,sizeof(flength),ofp);//д����Ϣ�ܳ��ȣ�������Ϣ�ı����ܳ��ȣ� 
    fwrite(&pt1, sizeof(long), 1, ofp);// д���ļ�ĩβ�����ļ���ֹλ�õ�ָ�� 
    fseek(ofp, pt1, SEEK_SET);
    fwrite(&n, sizeof(long), 1, ofp);//�ַ����� 
    for (i = 0; i < n; i ++){
		tmp = header[i];
 
        fwrite(&(header[i].b), 1, 1, ofp);//д���ַ� 
		pt1++;
        c = strlen(header[i].bits);
        fwrite(&c, 1, 1, ofp);//д����볤�� 
		pt1++;
        j = strlen(header[i].bits);
 
        if (j % 8 != 0){                                            //��λ������8ʱ���Ը������в������
            for (f = j % 8; f < 8; f ++){
            	strcat(header[i].bits, "0");
			}
        }
 
        while (header[i].bits[0] != 0){
            c = 0;
            for (j = 0; j < 8; j ++){
                if (header[i].bits[j] == '1'){
                	c = (c << 1) | 1;
				}
                else{
                	c = c << 1;
				}
            }
            strcpy(header[i].bits, header[i].bits + 8);
            fwrite(&c, 1, 1, ofp);                                            //�����õı�����Ϣд���ļ�
			pt1++;
        }
		header[i] = tmp;
    }
    fclose(ifp);
    fclose(ofp);                                                              //�ر��ļ�
 	cout<<"ѹ�����ļ�Ϊ:"<<outputfile<<endl;
	cout<<"ѹ�����ļ���:"<<pt1+4<<"�ֽ�"<<endl;  
    return 1;                                       //����ѹ���ɹ���Ϣ
}

 
 
//������uncompress()
//���ã���ѹ���ļ���������ѹ�������д�����ļ�
int uncompress(const char *filename,const char *outputfile){
    char buf[255], bx[255];
    unsigned char c;
	char out_filename[512];
    long i, j, m, n, f, p, l;
    long flength;
	int per = 10;
	int len = 0;
    FILE *ifp, *ofp;
	char c_name[512] = {0};
    ifp = fopen(filename, "rb");                                              //���ļ�
    if (ifp == NULL){
        return 0;     //����ʧ�ܣ������������Ϣ
    }
 
													  //��ȡԭ�ļ���
	if(outputfile){
		strcpy(out_filename,outputfile);
	}	
	else{
		strcpy(out_filename,c_name);
	}

    ofp = fopen(out_filename, "wb");                                            //���ļ�
    if (ofp == NULL){
        return 0;
    }
 
	fseek(ifp,0,SEEK_END);
	len = ftell(ifp);
	fseek(ifp,0,SEEK_SET);
 	cout << "��Ҫ��ȡ��ѹ���ļ�:" << filename << endl;
 	cout << "��ǰ�ļ���:" << len << "�ֽ�" << endl; 
	  
    fread(&flength, sizeof(long), 1, ifp);                                    //��ȡԭ�ļ���
    fread(&f, sizeof(long), 1, ifp);
    fseek(ifp, f, SEEK_SET);
    fread(&n, sizeof(long), 1, ifp);                                          //��ȡԭ�ļ�������
    for (i = 0; i < n; i ++){                                                  //��ȡѹ���ļ����ݲ�ת���ɶ�������
        fread(&header[i].b, 1, 1, ifp);                                       //��ȡ�ַ�
        fread(&c, 1, 1, ifp);                                                 //��ȡ�ַ���Ӧ���볤��
        p = (long) c;
        header[i].count = p;                                                  //�����볤�ȴ洢��header[i].count
        header[i].bits[0] = 0;
        if (p % 8 > 0){
        	m = p / 8 + 1;													  //mΪ������ռ�ֽ���
		}                                  
        else{
        	m = p / 8;
		} 
        for (j = 0; j < m; j ++){                                            //���ֽڶ�ȡ����
            fread(&c, 1 , 1 , ifp);                                           //��ȡ�����Ӧ��ASCLL��
            f = c;
            _itoa(f, buf, 2);                                                 //ASCLL��ת��Ϊ�������ַ������洢��buf��
            f = strlen(buf);                                                  //fΪ�ַ�������
            for (l = 8; l > f; l --){
                strcat(header[i].bits, "0");                                  //λ�����㣬ִ�в����������ÿ���ֽ���ǰ�油0
            }
            strcat(header[i].bits, buf);                                      //����λ��ӵ�header[i].bitsĩβ
        }
        header[i].bits[p] = 0;
    }
 
    for (i = 0; i < n; i ++){                                                  //��header[].bits��λ����������λ����С�ĳ���Ƶ�ʸ��ߣ��±��С���ڱ���
        for (j = i + 1; j < n; j ++){
            if (strlen(header[i].bits) > strlen(header[j].bits)){
                tmp = header[i];
                header[i] = header[j];
                header[j] = tmp;
            }
        }
    }

    p = strlen(header[n-1].bits);                                             //pΪbits��������λ��
    fseek(ifp, 8, SEEK_SET);
    m = 0;                                                                    //�ѽ�ѹ���ַ�����
    bx[0] = 0;
 
 
    while (1){
        while (strlen(bx) < (unsigned int)p){                                  //��ȡ����pλ
            fread(&c, 1, 1, ifp);
            f = c;
            _itoa(f, buf, 2);												  //����ת��Ϊ�ַ��� 
            f = strlen(buf);
            for (l = 8; l > f; l --){
                strcat(bx, "0");                                              //λ�����㣬ִ�в����������ÿ���ֽ���ǰ�油0
            }
            strcat(bx, buf);                                                  //����λ��ӵ�bxĩβ
        }
        for (i = 0; i < n; i ++){
            if (memcmp(header[i].bits, bx, header[i].count) == 0){			  //��bx��ǰheader[i].countλ��header[i].bits�Ƚϣ������ͬ��break
            	break;
			}
		}	
        strcpy(bx, bx + header[i].count);                                     //bx����header[i].countλ
        c = header[i].b;                                                      //�ҵ������Ӧ���ַ�
        fwrite(&c, 1, 1, ofp);                                                //д��c
        m ++;                                                                 //�ѽ�ѹ���ַ�����++
        if (m == flength){
        	break;
		} 
    }
    fclose(ifp);
    fclose(ofp);
 	cout << "��ѹ���ļ�Ϊ:" << out_filename << endl;
 	cout << "��ѹ���ļ���:" << flength <<"�ֽ�" << endl; 
    return 1;                   //����ɹ���Ϣ
}
 
int main()
{
	memset(&header,0,sizeof(header));
    memset(&tmp,0,sizeof(tmp));
 	char *a = "ziptxt";
 	char *b = "unziptxt";
 	char filenameToCompress[200];//��Ҫѹ�����ļ��� 
 	char compressFilename[200];//ѹ���ļ����� 
 	char filenameToUncompress[200];//��Ҫ��ѹ���ļ��� 
 	char uncompressFilename[200];//��ѹ�ļ����� 
 	char order[200];//ָ�� 
 	while(cin >> order){
 		if(strcmp(order, a) == 0){
 			cin >> filenameToCompress >> compressFilename; 
 			compress(filenameToCompress, compressFilename);
		}
		if(strcmp(order, b) == 0){
			cin >> filenameToUncompress >> uncompressFilename;
			uncompress(filenameToUncompress, uncompressFilename);	
		}
	}
	return 0;
}
