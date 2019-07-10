#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <direct.h>
#include <io.h>

// 학생 구조체 정의
typedef struct {
	// 구조체 사이즈 : 20bytes(align 2bytes)
	int num;
	char name[10];
	int score;
} Student;

// 함수 전방선언
Student* makeRandomStudent();
Student* readDB(char*);
void writeDB(char*, Student*);
void insert(char*, char*, char*, Student*);
void delete(char*, Student*);
void list(char*, Student*);
void sortByName(Student*);
void sortByScore(Student*);

int main() {
	srand(time(NULL));
	Student* students  = makeRandomStudent();
	while (1) {
		char commandLine[100];
		printf("명령어를 입력하세요:");
		fgets(commandLine, 100, stdin);
		commandLine[strlen(commandLine) - 1] = '\0';

		if (!strcmp(commandLine, "exit")) {
			printf("프로그램을 종료합니다.\n");
			return 0;
		}

		// 명령어 파싱
		char* firstCommand = strtok(commandLine, " ");

		// 파싱 된 명령어를 담을 변수들 선언
		char* dbName;
		char* num;
		char* name;
		char* score;
		char* etc;

		// 어떤 명령어인지 구분
		if (!strcmp(firstCommand, "read")) {
			// read 명령
			dbName = strtok(NULL, " ");
			students = readDB(dbName);
		}
		else if (!strcmp(firstCommand, "write")) {
			// write 
			dbName = strtok(NULL, " ");
			writeDB(dbName, students);
		}
		else if (!strcmp(firstCommand, "insert")) {
			// insert 명령
			num = strtok(NULL, " ");
			name = strtok(NULL, " ");
			score = strtok(NULL, " ");
			clock_t start, end;
			start = clock();
			insert(num, name, score, students);
			end = clock();
			printf("%lf초 걸렸습니다.\n",  (double)(end - start) / CLOCKS_PER_SEC);
		}
		else if (!strcmp(firstCommand, "delete")) {
			// delete 명령
			name = strtok(NULL, " ");
			clock_t start, end;
			start = clock();
			delete(name, students);
			end = clock();
			printf("%lf초 걸렸습니다.\n",(double)(end - start) / CLOCKS_PER_SEC);
		}
		else if (!strcmp(firstCommand, "list")) {
			// list 명령
			name = strtok(NULL, " ");
			list(name, students);
		}
		else if (!strcmp(firstCommand, "sort")) {
			// sort 명령
			etc = strtok(NULL, " ");
			clock_t start, end;
			start = clock();
			if (!strcmp(etc, "name")) sortByName(students);
			else if (!strcmp(etc, "score")) sortByScore(students);
			else printf("잘못된 정렬 방식입니다.\n");
			end = clock();
			printf( "%lf초 걸렸습니다.\n", (double)(end - start) / CLOCKS_PER_SEC);
		}
		else {
			//예외발생
			printf("예외 발생\n");
		}

	}
}
Student* makeRandomStudent() {
	Student* students = (Student*)malloc(sizeof(Student) * 10000);
	for (int i = 0; i < 10000; ++i) {
		students[i].num = i + 1;
		for (int j = 0; j < 9; j++) {
			students[i].name[j] = rand() % 26 + 'a';
		}
		students[i].name[9] = '\0';
		students[i].score = rand() % 101;
	}
	return students;
}
Student* readDB(char* dbName) {
	//파일 존재 여부 검사
	// access는 파일이 이미 존재하는지 검사할 수 있는 함수.
	if (!access(dbName, 00)) {
		//파일이 존재하는 경우 파일을 읽는다.
		printf("DB를 찾았습니다.\n");
		Student* students = (Student*)malloc(0);
		FILE* fp = fopen(dbName, "r");
		char studentInfo[100];
		int i = 0;
		while (fgets(studentInfo, 100, fp) != NULL) {
			// 새로 추가할 학생 메모리 할당
			students = (Student*)realloc(students, _msize(students) + sizeof(Student));
			char* num = strtok(studentInfo, " ");
			students[i].num = atoi(num);
			char* name = strtok(NULL, " ");
			strcpy(students[i].name, name);
			char* score = strtok(NULL, " ");
			students[i].score = atoi(score);
			i++;
		}
		fclose(fp);
		printf("DB를 읽었습니다.\n");
		return students;
	}
	else {
		printf("DB가 존재하지 않습니다.\n");
	}


}

void writeDB(char* dbName, Student* students) {
	FILE* fp = fopen(dbName, "w");
	for (int i = 0; i < _msize(students) / sizeof(Student); i++) {
		fprintf(fp, "%d %s %d\n", students[i].num, students[i].name, students[i].score);
	}
	fclose(fp);
	printf("DB를 저장했습니다.\n");
}

void insert(char* num, char* name, char* score, Student* students) {
	students = (Student*)realloc(students, _msize(students) + sizeof(Student));
	int index = (_msize(students) / sizeof(Student)) - 1;
	students[index].num = atoi(num);
	strcpy(students[index].name, name);
	students[index].score = atoi(score);
	printf("학생 정보를 삽입했습니다.\n");
}

void delete(char* name, Student* students) {
	for (int i = 0; i < _msize(students) / sizeof(Student); i++) {
		if (!strcmp(students[i].name, name)) {
			for (int j = i; j < _msize(students) / sizeof(Student)-1; j++) {
				students[j].num = students[j + 1].num;
				strcpy(students[j].name, students[j + 1].name);
				students[j].score = students[j + 1].score;
			}
			students = (Student*)realloc(students, _msize(students) - sizeof(Student));
			printf("학생 정보를 삭제했습니다.\n");
			return;
		}
	}
}

void list(char* name, Student* students) {
	for (int i = 0; i < _msize(students) / sizeof(Student); i++) {
		if (!strcmp(students[i].name, name)) {
			printf("번호 : %d\n이름 : %s\n점수 : %d\n", students[i].num, students[i].name, students[i].score);
			return;
		}
	}
	printf("찾고자 하는 학생이 없습니다.\n");
	return;
}

void sortByName(Student* students) {
	int smallStudent;
	for (int i = 0; i < _msize(students) / sizeof(Student) - 1; i++) {
		smallStudent = i;
		for (int j = i; j < _msize(students) / sizeof(Student); j++) {
			int shortName;
			if (strlen(students[j].name) > strlen(students[smallStudent].name))
				shortName = strlen(students[smallStudent].name);
			else
				shortName = strlen(students[j].name);
			for (int k = 0; k < shortName; k++) {
				if (students[j].name[k] < students[smallStudent].name[k])
					smallStudent = j;
				break;
			}
		}
		Student buffer;
		buffer.num = students[i].num;
		strcpy(buffer.name, students[i].name);
		buffer.score = students[i].score;
		students[i].num = students[smallStudent].num;
		strcpy(students[i].name, students[smallStudent].name);
		students[i].score = students[smallStudent].score;
		students[smallStudent].num = buffer.num;
		strcpy(students[smallStudent].name, buffer.name);
		students[smallStudent].score = buffer.score;
	}
	printf("DB를 이름순으로 정렬했습니다.\n");
}

void sortByScore(Student* students) {
	int smallStudent;
	for (int i = 0; i < _msize(students) / sizeof(Student) - 1; i++) {
		smallStudent = i;
		for (int j = i; j < _msize(students) / sizeof(Student); j++) {
			if (students[j].score < students[smallStudent].score) {
				smallStudent = j;
			}
		}
		Student buffer;
		buffer.num = students[i].num;
		strcpy(buffer.name, students[i].name);
		buffer.score = students[i].score;
		students[i].num = students[smallStudent].num;
		strcpy(students[i].name, students[smallStudent].name);
		students[i].score = students[smallStudent].score;
		students[smallStudent].num = buffer.num;
		strcpy(students[smallStudent].name, buffer.name);
		students[smallStudent].score = buffer.score;
	}
	printf("DB를 점수순으로 정렬했습니다.\n");
}
