## **핵심 API로 배우는 윈도우 프로그래밍(강경우, 한빛아카데미)**

![img](https://github.com/wjdgh283/WIN32API_Study/blob/master/image%20src/0-1.JPG)

![img](https://github.com/wjdgh283/WIN32API_Study/blob/master/image%20src/0-2.JPG)

### 1.윈도우 프로그래밍 기초

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





------

### 2.윈도우 기본 입출력

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





------

### 3.제어 메시지 처리하기

**■ 마우스에서 발생하는 메시지** 

- WM_LBUTTONDOWN, WM_LBUTTONUP, WM_MOUSEMOVE, WM_RBUTTONDOWN, WM_RBUTTONUP 등
- LOWORD(lParam), HIWORD(lParam) : 마우스 포인터 x,y 위치얻어올 때 사용 
- SetROP2() : 윈도우에 그림을 그릴 때 배경색과 그리는 색 사이에 수행할 연산을 지정
- R2_XORPEN을 래스터 연산으로 지정시 hdc에 등록된 색과  윈도우 배경색을 XOR연산하므로 같은 도형을 같은 위치에 두번 그리면 삭제하는 효과가 나타난다

■ **래스터연산** 

-  XOR연산 : 흰색 XOR 흰색 = 검은색	         

   ​		  검은색 XOR 흰색 = 흰색

-  래스터연산 이용시 원래 그림을 삭제하여 다시 그리기를 할 필요가 없다





------

### 4.윈도우 메뉴

■ **리소스 :** 윈도우 프로그램에서 화면을 통해 사용자에게 입력받고  출력하는 코드 외의 부분

■ **메뉴항목에서  WM_COMMAND메시지 발생 -> iMsg에 저장되어 WndProc()에 전달**

![img](https://github.com/wjdgh283/WIN32API_Study/blob/master/image%20src/4-1.JPG)

■ tchar_t \*fgetts(tchar_t *str, int n, FILE *fPtr) : fPtr파일에서 한 줄을 읽어와 str에 저장. 파일의 끝을 만나면 NULL반환

■ int *fputts(const tchar_t *str, FILE *fPtr) : str문자열을 fPtr파일에 출력함

■ errno-t _tfopen_s(FILE fPtr, const tchar_t *filename, const tchar_t *mode) :*열고자하는 filename으로 파일을 연후 fPtr에 연 파일 포인터저장

■ int _stprintf_s(tchar_t (&buffer)[size], const tchar_t *format, argument…) : format이 가리키는 문자열을 buffer공간으로 복사한다. 반환값은 출력한 문자열의 길이이고 오류 발생시 -1반환.

■ int _tcscmp(const tchar_t *string1, const tchar_t *string2) : string1,string2을 비교 -> string1-string2 

■ char *strtok_s(char *strToken, const char *strDelimit, char context) : 문자열 자르기





------

### 5.단축키와 비트맵

■ **단축키 설정단계**

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





------

### 6.대화상자와 컨트롤

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

■ **콤보박스 컨트롤** : 항목의 리스트 컨트롤을 보여주어 이 중 하나를 선택할 때 사용

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

■ **리스트박스 컨트롤** : 외부 입력 없이 여러 개의 항목을 일렬로 보여주는 컨트롤

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

  ​	} NMHDR;		

  (LVN_ITEMCHANGING : 리스트 컨트롤의 항목에 변화 발생

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







------

### 7.MDI 프로그래밍

![img](https://github.com/wjdgh283/WIN32API_Study/blob/master/image%20src/7-1.JPG)

■ MDI 프로그램 작성 순서

1. **리소스 편집**
   - 자식 윈도우를 생성하는 데 사용할 메뉴 항목을 추가
2. **WinMain() 함수 작성**
   - WinMain() 함수에 프레임 윈도우와 자식 윈도우를 위한 윈도우 클래스 변수를 선언, 	각 변수의 필드에 값 대입
   - 만든 윈도우 클래스 변수를 이용해 윈도우 클래스를 2개 등록(RegisterClass)
   - 프레임 윈도우 생성(CreateWindow,ShowWindow)
3. **WndProc() 함수 작성 : 자식 윈도우 생성하기**
   - 클라이언트 윈도우를 생성하는 코드 추가(클래스 변수설정, CreateWindow,ShowWindow)
   - 메뉴 항목을 선택했을 때 클라이언트 윈도우가 자식 윈도우를 생성하도록 메시지를	전송하는 코드 작성(클래스변수설정,SendMessage)
4. **ChildWndProc() 함수 작성 **
   - 자식 윈도우에서 발생하는 메시지를 처리하는 코드 작성

■ 컨트롤 윈도우 활용하기 : 컨트롤도 윈도우이지만 독립적으로 존재하지 않고 부모윈도우에 속한 자식 윈도우의 형태이다. 부모윈도우는 어떤 윈도우라도 가능하다.

- 리치 에디트 컨트롤 윈도우에 보내는 iMsg

  - EM_SETCHARFORMAT : CHARFORMAT 구조체에 설정한 글꼴을 리치 에디트 컨트롤의 텍스트에 적용		-wParam : 글꼴 변화를 적용할 영역을 알려주는 플래그값

    ​		SCF_ALL : 컨트롤에 있는 모든 텍스트에 적용

    ​		SCF_SELECTION : 현재 석택된 텍스트 영역에 적용

    ​		SCF_WORD : 선택된 문자에 적용

    -lParam : CHARFORMAT 구조체 값에 대한 포인터

  - EM_GETCHARFORMAT : 리치에디트 컨트롤의 텍스트 글꼴 정보를 CHARFORMAT 구조체에 얻어온다.

    -wParam : 얻어오고자 하는 것을 알려주는 플래그 값

    ​		SCF_DEFAULT : 기본값으로 설정한 글꼴 정보

    ​		SCF_SELECTION : 선택 영역의 글꼴 정보

    -lParam : CHARFORMAT 구조체 값에 대한 포인터 (메시지를 전송하면 구조체에 얻어온 글꼴정보가 저장됨)

  - EM_GETSELTEXT: 리치에디트 컨트롤에서 선택한 텍스트 영역을 버퍼에 복사해 오기 위해 보냄	

    -wParam : x

    -lParam : 버퍼의 시작주소값 (버퍼크기>선택한 텍스트 크기)

    -반환값 : 복사한 텍스트의 길이

  - EM_SETBKGNDCOLOR : 리치에디트 컨트롤의 배경 색을 변경하기 위해 보냄

    -wParam : 0아니면 -> 배경색으로 부모윈도우 배경색을 이용

    ​		   0이면 -> 변경하고자 하는 색상정보를  COLORREF 형식으로 lParam을 통해 제공

  - EM_SETFONTSIZE : 선택한 텍스트의 글꼴 크기를 조정

    -wParam : 글꼴 크기 정로수로 전달

    -lParam : 0





------

### 8.파일 입출력

■ **파일 다루기**

작업 디렉터리 : 읽어들이거나 쓰는 파일이 저장될 디렉터리 

순서 

1. 파일 만들기/열기(CreateFile())


2. 파일 읽기(ReadFile()-파일의 내용을 버퍼에 저장) / 파일 쓰기(WriteFile()-버퍼의 내용을 파일에 저장)
3. 파일 닫기(CloseHandle())

■ **파일 이름을 여러 개 저장하는 형식**

static char filepath[1000];

static char folder[100];

static char filename[1000];

LPSTR pStr;



OFN.lpstrFile = filepath;

OFN.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;

![img](https://github.com/wjdgh283/WIN32API_Study/blob/master/image%20src/8-1.JPG)





------

### 9.윈도우 소켓 프로그래밍

■ **프로젝트 환경에 라이브러리 추가**

​	프로젝트 - 속성 - 구성속성 - 링커 - 명령줄 - ws32_32.lib추가

■ **소켓 연결하기**

​	소켓 : IP주소와 포트번호로 이루어진 컴퓨터 통신의 끝점. 표준화된 인터페이스를 제공

* **소켓 프로그래밍을 위한 구조체**

  * 윈도우 소켓 초기화 정보를 가지고 있는 구조체

    typedef struct WSAData {

    ​	WORD  wVersion;

    ​	WORD                    wHighVersion;

    ​	char                        szDescription[WSADESCRIPTION_LEN+1];

    ​	char                        szSystemStatus[WSASYS_STATUS_LEN+1];

    ​	unsigned short        iMaxSockets;

    ​	unsigned short        iMaxUdpDg;

    ​	char FAR \*              lpVendorInfo;

    };

  * 소켓을 연결할 대상의 주소를 쓰는 구조체

    struct sockaddr_in {

    ​	short   sin_family;

    ​	u_short sin_port;

    ​	struct  in_addr sin_addr;

    ​	char    sin_zero[8];

    };

* **소켓 통신을 위한 함수**

  * 윈속 사용시작하기(서버,클라)

    int FAR WSAStartup {

    ​	WORD wVersionRequired,

    ​	LPWSADATA lpWSAData

    };

  * **소켓 생성하기(서버,클라)**

    SOCKET WSAAPI socket {

    ​	int af,		//address family 명시. AF_INET 이용

    ​	int type, 		//소켓 유형. SOCK_STREAM 이용

    ​	int protocol	//0 입력

    };

  * 주소와 소켓 연결하기(서버)

    BOOL bind {

    ​	SOCKET s,					//연결할 소켓

    ​	const SOCKADDR\* lpSockAddr,	//소켓에 지정할 주소와 포트 번호를 포함하는

    ​								//SOCKADDR구조체 주소

    ​	int nSockAddrLen			//주소를 저장하는 구조체 SOCKADDR크기

    };

  * 연결 요구 기다리기(서버)

    int PASCAL FAR listen {

    ​	SOCKET s,		//기다릴 소켓

    ​	int backlog		//대기할 수 있는 요구 최대 개수 1~5

    };

  * 연결 요구하기(클라)

    BOOL connect {

    ​	SOCKET s,						//연결에 사용할 소켓

    ​	struct SOCKADDR\* lpSockAddr,	//상대의 주소와 포트번호

    ​	int nSockAddrLen				//주소를 저장하는 구조체 SOCKADDR크기

    };

  * 연결 요구 받아들이기(서버)

    SOCKET accept {

    ​	SOCKET s,			//소켓에 대한 요구를 받아들임

    ​	SOCKADDR\* lpSockAddr,	//접속하는 상대방의 주소를 저장할 SOCKADDR구조체 주소

    ​	int\* lpSockAddrLen		//주소를 저장하는 구조체 SOCKADDR**

    };-> 클라이언트와 연결할 소켓을 반환한다.

  * 소켓과 관련된 리소스 해제하기(서버,클라)

    int closesocket(SOCKET s);

  * 윈속 사용 끝내기(서버,클라)

    int WSACleanup();

■ **메시지 교환하기**

- **메시지 송수신 함수**

  - 메시지 전송하기

    int send {

    ​	SOCKET s,		//연결된 소켓

    ​	const void\* lpBuf,	//보낼 메시지가 저장된 버퍼

    ​	int nBufLen,		//메시지의 길이

    ​	int nFlags		//0

    };-> 보낸 메시지 길이 반환

  - 메시지 수신하기

    int recv{

    ​	SOCKET s,		//연결된 소켓

    ​	void\* lpBuf,	//받는 메시지가 저장된 버퍼

    ​	int nBufLen,		//버퍼의길이

    ​	int nFlags		//0

    }; -> 받은 메시지 길이 반환

![img](https://github.com/wjdgh283/WIN32API_Study/blob/master/image%20src/9-1.JPG)

- **문자집합 변환 함수**

  - 멀티바이트-> 유니코드 변환

    int MultiByteToWideChar{

    ​	In 	UINT CodePage,				//ANSI문자열에 대한 언어. CP_ACP

    ​	In 	DWORD dwFlags,				//0

    ​	In 	LPCSTR lpMultiByteStr,		//변환하려는 멀티바이트 문자열

    ​	In 	int cbMultiByte,				//문자열의 길이. -1이면 자동계산

    ​	Out_opt	LPWSTR lpWideCharStr,	//유니코드로 변환 후 저장될 공간
    ​	In 	int cchWideChar				//유니코드 문자열의 길이

    }; 

    *5,6 번째를 NULL로 설정시 실제로 변환하지 않고 변환했을 경우의 유니코드 문자열 길이 값을 반환

  - 유니코드 -> 멀티바이트 변환

    int WideCharToMultiByte{

    ​	In 	UINT CodePage,				//ANSI문자열에 대한 언어. CP_ACP

    ​	In 	DWORD dwFlags,				//0

    ​	In 	LPCSTR lpWideCharStr,		//변환하려는 유니코드문자열

    ​	In 	int cchWidChar,				//문자열의 길이. -1이면 자동계산

    ​	Out_opt	LPWSTR lpMultiByteStr,	//멀티바이트로 변환 후 저장될 공간
    ​	In 	int cbMultiByte				//변환 후 문자열의 길이

    ​	In_opt_	LPCSTR lpDefaultChar,	//NULL

    ​	Out_opt	LPBOOL lpUsedDefaultChar //NULL

    }; 

    *5,6 번째를 NULL로 설정시 실제로 변환하지 않고 변환했을 경우의 멀티바이트 문자열 길이 값을 반환

■ **논블로킹 통신**

- **논블로킹 통신 :** 서버가 무한정 기다리지 않고 다른 일을 하다가 연결을 시도하는 신호가 도착하거나 통신 메시지가 도착하면 사용자가 정의한 윈도우 메시지를 보내는 방식

- 윈도우 메시지, 네트워크 이벤트 등록 함수

  int WSAAsyncSelect{

  ​	SOCKET s,			//연결된 소켓

  ​	HWND hwnd,		//메시지가 발생하는 윈도우 핸들

  ​	unsigned int iMsg,	//등록될 윈도우 메시지

  ​	long IEvent			//등록될 네트워크 이벤트

  }; -> 서버소켓에서 상대방에 의해 네트워크 이벤트가 발생하면 윈도우 메시지(WM_ASYNC)를 hwnd윈도우에 발생시키기 위해 설정한다. 

![img](https://github.com/wjdgh283/WIN32API_Study/blob/master/image%20src/9-2.JPG)





------

### 10.멀티스레드

-  **스레드** : 컴퓨터가 연속된 명령어를 차례로 하나씩 처리하는데, 이렇게 처리되는 명령어의 흐름. 어떠한 프로그램 내에서, 특히 프로세스 내에서 실행되는 흐름의 단위를 말한다. 일반적으로 한 프로그램은 하나의 스레드를 가지고 있지만, 프로그램 환경에 따라 둘 이상의 스레드를 동시에 실행할 수 있다. 이러한 실행 방식을 멀티스레드라고 한다.

![img](https://github.com/wjdgh283/WIN32API_Study/blob/master/image%20src/10-1.JPG)

- **프로젝트 설정**

  1. C런타임 라이브러리 process.h 파일 포함 -> _beginthreadex() 함수를 이용하여 Win32 API환경에서 멀티스레드 프로그램 작성을 위해


  2. 프로젝트 환경에 멀티스레드 DLL 설정 -> 프로젝트속성 - 구성속성 - C/C++ - 코드 생성 - 런타임 라이브러리 - 다중스레드 디버그 DLL(/MDd)로 설정

■ **스레드 생성하기**

- **스레드 생성 및 종료 함수**

  - 스레드 생성하기

    int WSAAsyncSelect{

    ​	uintptr_t	_beginthreadex(			//스레드 핸들 값 반환

    ​	void\*	*Security,				//SECURITY_ATTRIBUTES 구조체의 포인터 변수, NULL사용

    ​	unsigned	_StackSize,			//스레드를 위한 스택의 크기, 0사용

    ​	unsgined	(\*start_address)(void *),	//스레드로 실행할 함수 이름(형변환필요!)

    ​	void\*	*ArgList,					//스레드 함수에 전달될 매개변수 주소

    ​	unsigned	InitFlag,				//스레드 상태 지정 값으로 실행하기 위해 0사용

    ​	unsigned\*	*thrdAddr			//스레드 ID를 받기 위한 32비트 포인터변수, NULL사용

    };

  - 스레드 종료하기

    void _endthreadex{

    ​	unsigned retval	//반환을 원하는 값

    };

  - 스레드 정리하기

    BOOL WINAPI CloseHandle{

    ​	HANDLE hObject

    };

■ **스레드 동기화하기**

- **이벤트** : 스레드간의 동기를 맞추는 데 사용되는 동기화 개체

- **이벤트 관련 함수**

  - 이벤트 생성하기

    CreateEvent{						//이벤트 핸들 값 반환

    ​	LPSECURITY_ATTRIBUTES lpEventAttributes,	//보안 속성, 보통 NULL사용

    ​	BOOL bManualReset,				//시그널/넌시그널 수동 변환 설정

    ​									//TRUE : SetEvent,ResetEvent로 이벤트

    ​									//값 수동변환 잘 해야함

    ​									//FALSE : WaitForSingleObject함수가

    ​									이벤트 변환을 자동으로 수행

    ​	BOOL bInitialState,				//초기상태

    ​									//TRUE:시그널 상태로 이벤트 생성

    ​									//FALSE:넌시그널 상태로 이벤트 생성

    ​	LPCWSTR lpName);				//이벤트 이름, 보통NULL사용

    };

  - 이벤트 설정하기

    SetEvent{				//이벤트를 시그널 상태로 변환

    ​	HANDLE hEvent);		//이벤트 핸들

  - 이벤트 재설정(리셋)하기

    BOOL ResetEvent{	//이벤트를 넌시그널 상태로 변환

    ​	HANDLE hEvent);	//이벤트 핸들

  - 단일 개체 대기하기

    DWORD WaitForSingleObject{

    ​	HANDLE hHandle,		//대기할 개체의 핸들

    ​	DWORD dwMilliseconds);	//대기 시간, INFINITE는 무한정 대기

    -> hHandle의 시그널상태이면 진행, 넌시그널상태이면 dwMilliseconds동안 대기, 여러개 스레드가 동시에 대기 중이면 이 중 하나만 대기에서 풀리고 나머지는 계속 대기

  - 복수 개체 대기하기

    DWORD WaitForMultipleObjects{

    ​	DWORD nCount,				//대기할 개체의 개수

    ​	const HANDLE \*lpHandles,	//기다릴 개체들의 핸들

    ​	BOOL bWaitAll,				//대기할 개체가 모두 기다려야 하는지 여부

    ​								//TRUE : 모든 개체가 시그널 상태가 되기를 대기	

    ​								//FALSE : 1개의 개체라도 시그널 상태가 되기를 대기

    ​	DWORD dwMilliseconds);		//대기 시간

    -> lpHandles개체가 모두 시그널상태인지 검사. 넌시그널상태이면 bWaitAll의 값에 따라 대기