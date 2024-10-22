#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <direct.h>
#include <io.h>

// 학생 구조체 정의
typedef struct Node{
	// 구조체 사이즈 : 20bytes(align 2bytes)
	int num;
	char name[10];
	int score;
	struct Node* left;
	struct Node* right;
} Student;

// 함수 전방선언
Student* makeRandomStudents();
Student* readDB(char*);
void writeDB(char*, Student*);
void insert(char*, char*, char*, Student*);
Student* delete(char*, Student*);
void list(char*, Student*);
void sortByName(Student*);
void sortByScore(Student*);

int main() {
	srand(time(NULL));
	Student* students = makeRandomStudents();
	while (1) {
		printf("명령어를 입력하세요:");
		char commandLine[100];
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
			printf(" %lf초 걸렸습니다.\n",  (double)(end - start) / CLOCKS_PER_SEC);
		}
		else if (!strcmp(firstCommand, "delete")) {
			// delete 명령
			name = strtok(NULL, " ");
			clock_t start, end;
			start = clock();
			students = delete(name, students);
			end = clock();
			printf("%lf초 걸렸습니다.\n", (double)(end - start) / CLOCKS_PER_SEC);
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
			printf(" %lf초 걸렸습니다.\n", (double)(end - start) / CLOCKS_PER_SEC);
		}
		else {
			//예외발생
			printf("적절하지 못한 명령어 입니다.\n");
		}

	}
}

Student* makeRandomStudents() {
		Student* students = (Student*)malloc(sizeof(Student));
		Student* ptr;
		ptr = students;
		ptr->num = 1;
		for (int j = 0; j < 9; j++) {
			ptr->name[j] = rand() % 26 + 'a';
		}
		ptr->name[9] = '\0';
		ptr->score = rand() % 101;
		ptr->left = NULL;
		ptr->right = NULL;
		for (int i = 1; i < 10000; ++i) {
			Student* newStudent = (Student*)malloc(sizeof(Student));
			newStudent->num = i + 1;
			for (int j = 0; j < 9; j++) {
				newStudent->name[j] = rand() % 26 + 'a';
			}
			newStudent->name[9] = '\0';
			newStudent->score = rand() % 101;
			newStudent->left = ptr;
			newStudent->right = NULL;
			ptr->right = newStudent;
			ptr = newStudent;
		}

	return students;
}

Student* readDB(char* dbName) {
	//파일 존재 여부 검사
	// access는 파일이 이미 존재하는지 검사할 수 있는 함수.
	if (!access(dbName, 00)) {
		//파일이 존재하는 경우 파일을 읽는다.
		printf("DB를 찾았습니다.\n");
		FILE* fp = fopen(dbName, "r");
		char studentInfo[100];
		Student* students = (Student*)malloc(sizeof(Student));
		Student* ptr= students;
		fgets(studentInfo, 100, fp);
		char* num = strtok(studentInfo, " ");
		char* name = strtok(NULL, " ");
		char* score = strtok(NULL, " ");
		ptr->num = atoi(num);
		strcpy(ptr->name, name);
		ptr->score = atoi(score);
		ptr->left = NULL;
		ptr->right = NULL;
		int i = 0;
		while (fgets(studentInfo, 100, fp) != NULL) {
			// 새로 추가할 학생 메모리 할당
			Student* newStudent = (Student*)malloc(sizeof(Student));
			num = strtok(studentInfo, " ");
			name = strtok(NULL, " ");
			score = strtok(NULL, " ");
			newStudent->num = atoi(num);
			strcpy(newStudent->name, name);
			newStudent->score = atoi(score);
			newStudent->left = ptr;
			newStudent->right = NULL;
			ptr->right = newStudent;
			ptr = newStudent;
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
	Student* ptr = students;
	while(ptr != NULL) {
		fprintf(fp, "%d %s %d\n", ptr->num, ptr->name, ptr->score);
		ptr = ptr->right;
	}
	fclose(fp);
	printf("DB를 저장했습니다.\n");
}

void insert(char* num, char* name, char* score, Student* students) {
	Student* ptr = students;
	while (ptr->right != NULL) {
		ptr = ptr->right;
	}
	Student* newStudent = (Student*)malloc(sizeof(Student));
	ptr->right = newStudent;
	newStudent->left = ptr;
	newStudent->num = atoi(num);
	strcpy(newStudent->name, name);
	newStudent->score = atoi(score);
	newStudent->right = NULL;
	printf("학생 정보를 삽입했습니다.\n");
}

Student* delete(char* name, Student* students) {
	Student* ptr = students;
	while(ptr != NULL) {
		if (!strcmp(ptr->name, name)) {
			if (students == ptr) {
				students = ptr->right;
				students->left = NULL;
			}
			else if (ptr->right == NULL) {
				ptr->left->right = ptr->right;
			}
			else {
				ptr->left->right = ptr->right;
				ptr->right->left = ptr->left;
			}
			//ptr->right = NULL;
			//ptr->left = NULL;
			//free(ptr);
			printf("학생 정보를 삭제했습니다.\n");
			return students;
		}
		ptr = ptr->right;
	}
	printf("찾고자 하는 학생이 없습니다.\n");
	return students;
}

void list(char* name, Student* students) {
	Student* ptr = students;
	while(ptr != NULL) {
		if (!strcmp(ptr->name, name)) {
			printf("번호 : %d\n이름 : %s\n점수 : %d\n", ptr->num, ptr->name, ptr->score);
			return;
		}
		ptr = ptr->right;
	}
	printf("찾고자 하는 학생이 없습니다.\n");
	return;
}

void sortByName(Student* students) {
	Student* start = students;
	Student* smallStudent = start;
	Student* ptr = start;
	while(start != NULL) {
		smallStudent = start;
		ptr = start->right;
		while(ptr != NULL) {
			int shortName;
			if (strlen(ptr->name) > strlen(smallStudent->name))
				shortName = strlen(smallStudent->name);
			else
				shortName = strlen(ptr->name);
			for (int k = 0; k < shortName; k++) {
				if (ptr->name[k] > smallStudent->name[k]) {
					break;
				}
				else if (ptr->name[k] < smallStudent->name[k]) {
					smallStudent = ptr;
					break;
				}
			}
			ptr = ptr->right;
		}

		Student buffer;
		buffer.num = start->num;
		strcpy(buffer.name, start->name);
		buffer.score = start->score;
		start->num = smallStudent->num;
		strcpy(start->name, smallStudent->name);
		start->score = smallStudent->score;
		smallStudent->num = buffer.num;
		strcpy(smallStudent->name, buffer.name);
		smallStudent->score = buffer.score;
		start = start->right;
	}
	printf("DB를 이름순으로 정렬했습니다.\n");
}

void sortByScore(Student* students) {
	Student* start = students;
	Student* smallStudent = start;
	Student* ptr = start;
	while(start != NULL) {
		smallStudent = start;
		ptr = start;
		while(ptr != NULL) {
			if (ptr->score < smallStudent->score)
				smallStudent = ptr;
			ptr = ptr->right;
		}
		Student buffer;
		buffer.num = start->num;
		strcpy(buffer.name, start->name);
		buffer.score = start->score;
		start->num = smallStudent->num;
		strcpy(start->name, smallStudent->name);
		start->score = smallStudent->score;
		smallStudent->num = buffer.num;
		strcpy(smallStudent->name, buffer.name);
		smallStudent->score = buffer.score;
		start = start->right;
	}
	printf("DB를 점수순으로 정렬했습니다.\n");
}
