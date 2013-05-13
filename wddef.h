#ifndef WDDEF_H
#define WDDEF_H

//summary of a request
typedef struct {
    char caMethod[15] ;	//request method
    char * pcHost ;			//host
    char * pcUrl ;			//url
}REQUSUM, *PREQUSUM ;

//summary of a response
typedef struct {
    char caCode[5] ;		//response code
    unsigned int iLenCon ;		//content-length
    char * pcType ;			//content type
}RESPSUM, *PRESPSUM ;

//one request and response info
typedef struct {
    char * pcRequRaw ;
    char * pcRespRaw ;
    unsigned int iLenRequ ;	//request packet length
    unsigned int iLenResp ;	//response packet length
    PREQUSUM pRequSum ;	//summary of a request package
    PRESPSUM pRespSum ;	//summary of a response package
}DATANODE, *PDATANODE ;

#endif // WDDEF_H
