#include<wordcut/wcdict.h>
#include<path.h>
int main()
{
  	char *str="มาก";
	WcDict dict;
	WcDictMap map;
	WcDictIterPos *pos;

	gint len=strlen(str),i,j,k,alen;

	wc_dict_init(&dict);
	printf ("!!!\n");
	wc_dict_load(&dict,"dict.etd");
	wc_dict_map_init(&map,&dict,str,len);
	printf ("%s\n",str);
	for(i=0;i<len;i++) {
		alen=wc_dict_map_assoc_len(&map,i);
		g_print ("i=%d\tlen=%d",i,alen);
		for(j=0;j<alen;j++) {
			g_print ("\t%d",wc_dict_map_assoc_at(&map,i,j));
			pos=wc_dict_map_assoc_pos_at(&map,i,j);
			for(k=0;k<pos->len;k++) {
				wc_byte id;
				const gchar *pos_str;
				wc_dict_iter_pos_posid(pos,k,&id);
				pos_str=wc_dict_iter_pos_posid2str(pos,id);
				g_print ("<%d,%s>",id,pos_str);
	    		}
		}
		g_print ("\n");
	}
	wc_dict_map_destroy(&map);
	wc_dict_destroy(&dict);
	return 0;
}
