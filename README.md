## **핵심 API로 배우는 윈도우 프로그래밍(강경우, 한빛아카데미)**

![img](https://github.com/wjdgh283/WIN32API_Study/blob/master/image%20src/0-1.JPG)

![img](https://github.com/wjdgh283/WIN32API_Study/blob/master/image%20src/0-2.JPG)

#### 1.윈도우 프로그래밍 기초

■ **윈도우 SDK** : 윈도우 API로 구현한 라이브러리나 헤더파일들

■ **윈도우 프로그램의 특징GUI기반 프로그램이벤트 기반 프로그램** : 윈도우가 이벤트를 감지하고 메시지를 보내면 윈도우 프로그램은 수신한 메시지에 따라 반응함멀티쓰레드 기반 프로그램

■ **윈도우 프로그램에서 메시지 처리과정**

![img](https://github.com/wjdgh283/WIN32API_Study/blob/master/image%20src/1-1.JPG)

3. 정수값 메시지가 큐에 쌓임

4. WinMain() 함수는 큐에서 메시지를 차례로 꺼냄

5. 꺼낸 메시지를 해석해 메시지 처리함수로 보냄. GetMessage() 함수로 메시지를 꺼낸 후메시지는 msg에 저장한다. TranslateMessage()함수로 변형하여 DispatchMessage()함수로 메시지를 WinProc()로 보냄.

6. 메시지를 받아서 반응을 보임

   ​

   ■ 윈도우 프로그램의 기본구조

   * 윈도우에서 발생한 메시지는 메시지 큐에 저장되고 메시지 처리 부분이 하나씩 처리
   * 메시지 처리부분은 처리할 메시지에 대해서만 작성하고, 나머지 메시지 처리는 윈도우 커널에서 일임





#### 2.윈도우 기본 입출력

■ **DC(device context)** : 출력을 위해 얻어온 화면 영역

■ **GetDC()함수로 디바이스 콘텍스트를 얻어와 출력한 다음에는 ReleaseDC()함수를 호출해 출력을 마쳤음을 알린다.**

■ **멀티바이트 문자집합 사용:** 한글은 2바이트에 저장, 영문자와 특수문자는 1바이트에 저장

​	ex)char str[15]=“나는 love”;

■ **유니코드 문자집합 사용:** 한글, 영문자, 특수문자 모두 2바이트에 저장

​	ex)WCHAR str[15]=L “나는 love”;

■ **TCHAR :** 프로젝트 속성에서 설정된 문자집합에 따라 문자열을 처리하는 부분을 멀티바이트 또는 유니코드로 자동변경해준다.

​	ex) TCHAR str[15]=_T(“나는 love”);

![img](https://github.com/wjdgh283/WIN32API_Study/blob/master/image%20src/2-1.JPG)

■ **키보드에서 발생하는 메시지** 

- 키보드 발생메시지는 WM_CHAR, WM_KEYDOWN, WM_KEYUP 
- wParam에 저장된 키 값은 가상키값으로 저장되어있다.

*타이머 ID구분 : wParam





#### 3.제어 메시지 처리하기

**■ 마우스에서 발생하는 메시지** 

- WM_LBUTTONDOWN, WM_LBUTTONUP, WM_MOUSEMOVE, WM_RBUTTONDOWN, WM_RBUTTONUP 등
- LOWORD(lParam), HIWORD(lParam) : 마우스 포인터 x,y 위치얻어올 때 사용 
- SetROP2() : 윈도우에 그림을 그릴 때 배경색과 그리는 색 사이에 수행할 연산을 지정
- R2_XORPEN을 래스터 연산으로 지정시 hdc에 등록된 색과  윈도우 배경색을 XOR연산하므로 같은 도형을 같은 위치에 두번 그리면 삭제하는 효과가 나타난다

■ **래스터연산** 

-  XOR연산 : 흰색 XOR 흰색 = 검은색	         

   ​		  검은색 XOR 흰색 = 흰색

-  래스터연산 이용시 원래 그림을 삭제하여 다시 그리기를 할 필요가 없다





#### 4.윈도우 메뉴

■ **리소스 :** 윈도우 프로그램에서 화면을 통해 사용자에게 입력받고  출력하는 코드 외의 부분

■ 메뉴항목에서  WM_COMMAND메시지 발생 -> iMsg에 저장되어 WndProc()에 전달

![img](https://github.com/wjdgh283/WIN32API_Study/blob/master/image%20src/4-1.JPG)

■ **tchar_t \*fgetts(tchar_t *str, int n, FILE *fPtr) :** fPtr파일에서 한 줄을 읽어와 str에 저장. 파일의 끝을 만나면 NULL반환

■ **int *fputts(const tchar_t *str, FILE *fPtr) :** str문자열을 fPtr파일에 출력함

■ **errno-t _tfopen_s(FILE fPtr, const tchar_t *filename, const tchar_t *mode) :** 열고자하는 filename으로 파일을 연후 fPtr에 연 파일 포인터저장

■ **int _stprintf_s(tchar_t (&buffer)[size], const tchar_t *format, argument…) :** format이 가리키는 문자열을 buffer공간으로 복사한다. 반환값은 출력한 문자열의 길이이고 오류 발생시 -1반환.

■ **int _tcscmp(const tchar_t *string1, const tchar_t *string2) :** string1,string2을 비교 -> string1-string2 

■ **char *strtok_s(char *strToken, const char *strDelimit, char context) :** 문자열 자르기





#### 5.단축키와 비트맵

**■ 단축키 설정단계  **

1. 메뉴 속성창의 Caption에 단축키 표시   

2. 새로운 액셀러레이터 추가   
3. 단축키 매핑   
4. 단축키 설정 코드 작성

■ **비트맵 이미지 출력과정**

1. **비트맵 가져오기** : 그림을 비주얼 스튜디오의 비트맵으로 불러온다
2. **비트맵 로드하기** : 비트맵의 ID와 LoadBitmap()함수를 이용해 비트맵 핸들에 비트맵 로드
3. **화면 얻기** : 디바이스 콘텍스트 얻기
4. **메모리 디바이스 콘텍스트 만들기** : CreateCompatibleDC()함수를 이용해 hdc와 호환되는 memdc(화면이 없고 메모리에만 존재) 만들기.
5. **비트맵 사용 선언** : SelectObject()함수를 이용해 hBitmap에 로드된 이미지를 memdc에 설정
6. **화면에 비트맵 출력** : memdc그림을 BitBlt(), StretchBlt()로 hdc에 옮김

■ **더블버퍼링**

![img](https://github.com/wjdgh283/WIN32API_Study/blob/master/image%20src/5-1.JPG)

1. 비트맵 이미지를 hBit2에 로드
2. 2 .hdc에서 호환이 되는 mem1dc만들고 mem1dc에서 호환이 되는 mem2dc만들기
3. hdc와 호환이 되도록 CreateCompatibleBitmap()함수를 이용해 hBit1만들기
4. mem1dc에 hBitmap1설정(그리기) , mem2dc에 hBitmap2설정(그리기)
5. mem2dc그림을 mem1dc에 옮기기
6. mem1dc에 텍스트 쓰기-> 화면에 출력할 내용이 mem1dc에 모두 출력완료
7. BitBlt()함수로 mem1dc내용을 hdc에 옮긴다. 

■ **애니메이션 출력**

​	AND연산으로 마스크출력 - OR연산으로 같은 자리에 이미지출력





#### 6.대화상자와 컨트롤

■ 대화상자 이용

1. 리소스에서 새로운 대화상자 만들기
2. 대화상자에 대한 메시지 처리 함수 작성하기
3. 대화상자 띄우기

**■ 컨트롤을 대화상자에서 기본적으로 사용할 수 있도록 제공되는 것**

**■ 컨트롤에서 대화상자로 오는 WM_COMMAND 메시지**

![img](https://github.com/wjdgh283/WIN32API_Study/blob/master/image%20src/6-1.JPG)

■ **컨트롤에 메시지를 전송하는 함수 (콤보박스, 리스트박스.. 등에서 사용)**	

​	LRESULT SendMessage(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam); 

​				- hwnd : 메시지를 받을 윈도우나 컨트롤의 핸들 값		

​				- iMsg : 전송할 메시지		
​				- wParam, lParam : 메시지의 부가 정보

■ **버튼 컨트롤**

- 통지 정보 종류
  - BN_CLICKED : 버튼클릭
  - BN_DBCLICK : 버튼 더블클릭
  - BN_DISABLE : 버튼 사용 불가능 상태
  - BN_HILITE : 사용자가 버튼 선택
  - BN_SETFOCUS : 버튼이 포커스를 받음
- 윈도우에 작은 윈도우가 존재하고, 그것은 GetDlgItem()함수를 이용해 ID로 얻어올 수 있다.

**■ 에디트 컨트롤** : 사용자의 키보드 입출력을 위한 편집 창

- 통지 정보 종류
  - EN_CHANGE : 에디트 컨트롤의 내용이 변함
  - EN_HSCROLL : 에디트 컨트롤의 수평 스크롤바 선택
  - EN_VSCROLL : 에디트 컨트롤의 수직 스크롤바 선택
  - EN_SETFOCUS : 에디트 컨트롤이 포커스를 받음

**■ 콤보박스 컨트롤 **: 항목의 리스트 컨트롤을 보여주어 이 중 하나를 선택할 때 사용

- 통지 정보 종류

  - CBN_DROPDOWN : 콤보박스 컨트롤에 등록된 항목이 아래로 펼쳐짐
  - CBN_DBCLK : 아래로 펼쳐진 항목 리스트 컨트롤에서 하나를 더블클릭
  - CBN_EDITCHANGE : 콤보박스 컨트롤의 텍스트 편집 공간에 텍스트를 추가,수정
  - CBN_SELCHANGE : 항목 리스트 컨트롤에서 하나를 선택

- 콤보 박스 컨트롤에 보내는 iMsg

  - CB_ADDSTRING : 콤보박스 컨트롤의 항목 리스트에 문자열을 새로 추가

    ​	wParam : 사용X

    ​	lParam : 추가될 문자열의 시작 주소

  - CB_DELETESTRING : 콤보박스 컨트롤에 있는 항목 하나를 삭제

    ​	wParam : 삭제하고자 하는 항목의 인덱스		

    ​	lParam : 0

  - CB_FINDSTRING : 항목 리스트 컨트롤에 있는 텍스트 중 원하는 텍스트를 찾음

    ​	wParam : 찾기 시작할 위치를 지정하는 인덱스 값으로 0부터 시작함. -1로 하면 리스트 컨트롤 전체

    ​			  에서 찾는다

    ​	lParam : 찾고자 하는 문자열의 시작 주소

  - CB_GETCOUNT : 콤보 박스 컨트롤의 항목 리스트에 있는 항목의 개수 얻기

    ​	wParam : 0		

    ​	lParam : 0

  - CB_GETCURSEL : 현재 선택한 항목의 인덱스 번호 얻기

    ​	wParam : 0		

    ​	lParam : 0

  - CB_SERCURSEL : 콤보박스 컨트롤의 텍스트 편집 공간에 지정한 항목의 텍스트를 보여줌

    ​	wParam : 나타내고자 하는 항목의 인덱스 번호

    ​	lParam : 사용x

**■ 리스트박스 컨트롤 **: 외부 입력 없이 여러 개의 항목을 일렬로 보여주는 컨트롤

- 통지 정보 종류

  - LBN_DBCLK : 리스트 박스 컨트롤의 여러 항목 중 하나를 더블클릭
  - LBN_SELCHANGE : 항목 중 하나가 선택됨
  - LBN_SETFOCUS : 리스트 박스 컨트롤이 포커스를 받음
  - LBN_KILLFOCUS : 리스트 박스 컨트롤이 포커스를 잃음
  - LBN_DELETEITEM : 리스트 박스 컨트롤의 항목 중 하나가 삭제됨

- 리스트 박스 컨트롤에 보내는 iMsg

  - LB_ADDSTRING : 리스트 박스 컨트롤의 항목 리스트 마지막에 문자열을 추가

    ​	wParam : 사용x

    ​	lParam : 문자열의 시작 주소	

  - LB_DELETESTRING : 리스트 박스 컨트롤에 있는 항목 중 하나를 삭제

    ​	wParam : 삭제하려는 항목의 인덱스 정수

    ​	lParam : 0	

  - LB_FINDSTRING : 항목 리스트 컨트롤에 있는 텍스트 중 원하는 텍스트 찾기

    ​	wParam : 찾기 시작할 위치를 지정하는 인덱스 값으로 0부터시작함. -1로 하면 리스트 컨트롤 전체에서 찾음

    ​	lParam : 찾고자 하는 문자열의  시작 주소	

  - LB_GETCOUNT : 리스트 박스 컨트롤의 항목 리스트에 있는 항목 개수 얻기

    ​	wParam : 0

    ​	lParam :0	

  - LB_GETCURSEL : 현재 선택한 항목의 인덱스 번호 얻기

    ​	wParam : 0

    ​	lParam :0	

  - LB_SETCURSEL : 선택한 항목을 보여줌

    ​	wParam : 선택한 항목의 인덱스 번호

    ​	lParam : 사용x	

  - LB_GETSELCOUNT : 리스트 컨트롤 중 선택한 항목의 개수 얻기

    ​	wParam :0

    ​	lParam :0	

  - LB_GETSELITEM : 리스트 컨트롤 중 선택한 항목의 인덱스 번호 가져오기

    ​	wParam : 선택할 수 있는 항목의 최대 개수

    ​	lParam : 선택한 항목들의 인덱스 번호를 저장할 정수 배열의 시작주소

  - LB_GETTEXT : 항목 리스트 컨트롤 중 wParam에서 지정한 인덱스 항목의 텍스트 얻어오기

    ​	wParam : 얻어올 항목의  인덱스 번호

    ​	lParam : 얻어온 텍스트를 저장할 버퍼의 시작 주소

  - LB_INSERTSTRING : 항목 리스트 중간에 문자열을 새로운 항목으로 추가

    ​	wParam : 항목 리스트 중 추가될 위치의 인덱스 번호

    ​	lParam : 문자열의 시작 주소

**■ 모덜리스 대화상자**

- 모덜 대화상자는 대화상자의 핸들 값이 필요없지만, 모덜리스 대화상자는 부모 윈도우와 대화상자가 동시에 구동되기 때문에 부모 윈도우가 대화상자의 윈도우 핸들 값을 가져야 한다.
- 모덜 대화상자 생성,삭제 : DialogBox(), EndDialog()
- 모덜리스 대화상자 생성,삭제 : CreateDialog(), DestroyWindow()

**■ 리스트 컨트롤 :** 리스트박스 컨트롤의 발전된 형태

**■ 컨트롤에서 대화상자로 오는 WM_NOTIFY메시지 :** 컨트롤에 변화가 있을 때 부모 윈도우(대화상자)에 보내진다

![img](https://github.com/wjdgh283/WIN32API_Study/blob/master/image%20src/6-2.JPG)

- NMHDR구조체

  ​	typedef struct tagNMHDR {

  ​			HWND 	hwndFrom; ->변화가 발생한컨트롤의 윈도우 핸들 값

  ​			UINT_PTR 	idFrom; ->메시지를 보낸 컨트롤의 ID값

  ​			UINT 		code; ->컨트롤에 발생한 변화 내용

  ​	} NMHDR;		(LVN_ITEMCHANGING : 리스트 컨트롤의 항목에 변화 발생

  ​					LVN_KEYDOWN: 리스트 컨트롤에서 키보드 누름

  ​					LVN_INSERTITEM: 리스트 컨트롤에 새항목 추가

  ​					LVN_DELETEITEM: 리스트 컨트롤의 항목 하나 삭제	

  ​					LVN_COLUMNCLICK: 리스트 컨트롤의 특정 칼럼 선택)

- NMLISTVIEW 구조체

  ​	typedef struct tagNMLISTVIEW {

  ​			NMHDR 	hdr; ->WM_NOTIFY와 함께 변화가 일어난 컨트롤의 윈도우 핸들 값과 변화내용을 알

  ​						   려주는 정보. 이는 lParam을 NMLISTVIEW로 형변환 할 때와 동일

  ​			int		iItem;->리스트 컨트롤의 몇 번째 행에 변화가 있었는지에 대한 인덱스 0부터 시작

  ​			int 		iSubItem; ->리스트 컨트롤의 몇 번째 열에 변화가 있었는지에 대한 인덱스 0부터시작

  ​			UINT 	uNewState; ->나머지는 WM_NOTIFY에서 사용x

  ​			UINT 	uOldState;		

  ​			UINT 	uChanged;

  ​			POINT 	ptAction;

  ​			LPARAM lParam;

  ​	} NMLISTVIEW, \*LPNMLISTVIEW;







#### 7.MDI 프로그래밍

![img](https://github.com/wjdgh283/WIN32API_Study/blob/master/image%20src/7-1.JPG)