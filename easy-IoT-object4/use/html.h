#ifndef __HTML_H
#define __HTML_H

const static char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";


//��¼����
const unsigned char html1[] ="\
    <!DOCTYPE html>\
    <html>\
    <head>\
    <title>��¼����</title>\
    </head>\
    <body>\
    <form action=\"sniper\"\
    style=\"margin-top:500px;margin-left:100px;\
    width:500px;height:350px;background-color:darkgray;\
    padding-top:30px;padding-left:70px;\">\
    <div style=\"padding-left:80px;\">\
    <h2 style=\"font-size:35px;\">�û���¼</h2></div>\
    <span style=\"font-size:26px;\">�û���:</span>\
      <input type=\"text\" name=\"user\" value=\"admin\" length=\"20\" style=\"width:300px;height:35px;\"/><br><br>\
      <span style=\"font-size:26px;\">��&nbsp;&nbsp;&nbsp;��:</span>\
      <input type=\"password\" name=\"pwd\" value=\"\" style=\"width:300px;height:35px;\"/><br><br>\
      <div style=\"margin-left:80px;margin-height=120px\">\
      <input type=\"reset\" value=\"����\" style=\"width:80px;height:35px;font-size:20px;margin-right:50px;border-radius:5px;\"/>\
      <input type=\"submit\" value=\"��¼\"style=\"width:80px;height:35px;font-size:20px;border-radius:5px;\"/>\
      </div>\
    </form>\
    </body>\
    </html>";

////·�����ý���    
const unsigned char html3[]="\
    <!DOCTYPE html>\
    <html>\
    <head>\
    <title>��¼����</title>\
    </head>\
    <style type=\"text/css\">\
    input{\
      height:27px;\
    }\
    </style>\
    <body>\
    <form action=\"sniper\" style=\"margin-top:500px;margin-left:100px;\
     width:500px;height:350px;background-color:darkgray;\
    padding-top:30px;padding-left:70px;font-size:24px;\">\
       <br><br>\
    SSID:&nbsp;&nbsp;<input type=\"text\" name=\"user\" value=\"\"/><br><br>\
    ����:     <input type=\"password\" name=\"pwd\" value=\"\"/><br><br>\
    <input type=\"reset\" value=\"����\" style=\"font-size:22px;height:35px;margin-left:50px;\"/>\
    <input type=\"submit\" value=\"����\" style=\"font-size:22px;height:35px;margin-left:50px;\"/>\
    </form>\
    </body>\
    </html>";




//const unsigned char html4[]="\
//    <!DOCTYPE html>\
//    <html>\
//    <head>\
//    <title>��¼����</title>\
//    </head>\
//    <body>\
//    <form action=\"sniper\" style=\"margin-top:200px;margin-left:650px;\
//    width:500px;height:350px;background-color:darkgray;\
//    padding-top:50px;padding-left:100px;font-size:24px;\"><br><br>\
//    IP&nbsp;&nbsp;&nbsp;��:<input type=\"text\" name=\"ipname\" value=\"\"\
//    style=\"height:30px;\"/><br><br>\
//    �˿ں�:<input type=\"text\" name=\"port\" value=\"\" style=\"height:30px;\"/><br><br>\
//    <input type=\"submit\" value=\"����\" style=\"height:35px;font-size:22px;margin-left:50px;\"/>\
//    <input type=\"reset\" value=\"����\" style=\"height:35px;font-size:22px;margin-left:50px;\"/>\
//    </form>\
//    </body>\
//    </html>";

//const unsigned char html5[]="\
//    <!DOCTYPE html>\
//    <html>\
//    <head>\
//    <title>��¼����</title>\
//    </head>\
//    <style type=\"text/css\">\
//    body{\
//       margin:auto 0;\
//      padding:0;\
//     font-size:20px;\
//    }\
//    form{\
//     width:440px;\
//     height:250px;\
//     background:darkgray;\
//    margin-left:35%;\
//    margin-top:10%;\
//    padding-left:200px;\
//    padding-top:80px;\
//      }\
//    #save{\
//      margin-left:70px;\
//     font-size:20px;\
//    height:35px;\
//    }\
//     #clear{\
//       margin-left:50px;\
//      font-size:20px;\
//    height:35px;\
//    }\
//    input{\
//      height:25px;\
//    }\
//    </style>\
//    <body>\
//    <form action=\"sniper\" >\
//    ����ԭ����:&nbsp;<input type=\"password\" name=\"oldPSD\" value=\"\"/><br><br>\
//    ����������:&nbsp;<input type=\"password\" name=\"newpsd\" value=\"\"/><br><br>\
//    ȷ��������:&nbsp;<input type=\"password\" name=\"newpsd2\" value=\"\"/><br><br>\
//    <input type=\"submit\" value=\"����\" id=\"save\"/>\
//    <input type=\"reset\" value=\"����\" id=\"clear\"/>\
//    </form>\
//    </body>\
//    </html>";

    
//·��������ɽ���
const unsigned char html6[] ="\
    <!DOCTYPE html>\
    <html>\
    <head>\
    <title>·��������ɽ���</title>\
    </head>\
    <body>\
    <form action=\"sniper\"\
    style=\"margin-top:500px;margin-left:200px;\
    width:500px;height:350px;background-color:darkgray;\
    padding-top:30px;padding-left:70px;\">\
    <div style=\"padding-left:80px;\">\
    <h2 style=\"font-size:35px;\">·�����óɹ�</h2></div>\
    </div>\
    </form>\
    </body>\
    </html>";

    
#endif
