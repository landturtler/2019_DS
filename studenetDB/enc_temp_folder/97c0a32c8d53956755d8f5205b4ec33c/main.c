#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <direct.h>
#include <io.h>

// �л� ����ü ����
typedef struct {
	// ����ü ������ : 20bytes(align 2bytes)
	int num;
	char name[10];
	int score;
} Student;

// �Լ� ���漱��
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
		printf("��ɾ �Է��ϼ���:");
		fgets(commandLine, 100, stdin);
		commandLine[strlen(commandLine) - 1] = '\0';

		if (!strcmp(commandLine, "exit")) {
			printf("���α׷��� �����մϴ�.\n");
			return 0;
		}

		// ��ɾ� �Ľ�
		char* firstCommand = strtok(commandLine, " ");

		// �Ľ� �� ��ɾ ���� ������ ����
		char* dbName;
		char* num;
		char* name;
		char* score;
		char* etc;

		// � ��ɾ����� ����
		if (!strcmp(firstCommand, "read")) {
			// read ���
			dbName = strtok(NULL, " ");
			students = readDB(dbName);
		}
		else if (!strcmp(firstCommand, "write")) {
			// write 
			dbName = strtok(NULL, " ");
			writeDB(dbName, students);
		}
		else if (!strcmp(firstCommand, "insert")) {
			// insert ���
			num = strtok(NULL, " ");
			name = strtok(NULL, " ");
			score = strtok(NULL, " ");
			clock_t start, end;
			start = clock();
			insert(num, name, score, students);
			end = clock();
			printf("%lf�� �ɷȽ��ϴ�.\n",  (double)(end - start) / CLOCKS_PER_SEC);
		}
		else if (!strcmp(firstCommand, "delete")) {
			// delete ���
			name = strtok(NULL, " ");
			clock_t start, end;
			start = clock();
			delete(name, students);
			end = clock();
			printf("%lf�� �ɷȽ��ϴ�.\n",(double)(end - start) / CLOCKS_PER_SEC);
		}
		else if (!strcmp(firstCommand, "list")) {
			// list ���
			name = strtok(NULL, " ");
			list(name, students);
		}
		else if (!strcmp(firstCommand, "sort")) {
			// sort ���
			etc = strtok(NULL, " ");
			clock_t start, end;
			start = clock();
			if (!strcmp(etc, "name")) sortByName(students);
			else if (!strcmp(etc, "score")) sortByScore(students);
			else printf("�߸��� ���� ����Դϴ�.\n");
			end = clock();
			printf( "%lf�� �ɷȽ��ϴ�.\n", (double)(end - start) / CLOCKS_PER_SEC);
		}
		else {
			//���ܹ߻�
			printf("���� �߻�\n");
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
	//���� ���� ���� �˻�
	// access�� ������ �̹� �����ϴ��� �˻��� �� �ִ� �Լ�.
	if (!access(dbName, 00)) {
		//������ �����ϴ� ��� ������ �д´�.
		printf("DB�� ã�ҽ��ϴ�.\n");
		Student* students = (Student*)malloc(0);
		FILE* fp = fopen(dbName, "r");
		char studentInfo[100];
		int i = 0;
		while (fgets(studentInfo, 100, fp) != NULL) {
			// ���� �߰��� �л� �޸� �Ҵ�
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
		printf("DB�� �о����ϴ�.\n");
		return students;
	}
	else {
		printf("DB�� �������� �ʽ��ϴ�.\n");
	}


}

void writeDB(char* dbName, Student* students) {
	FILE* fp = fopen(dbName, "w");
	for (int i = 0; i < _msize(students) / sizeof(Student); i++) {
		fprintf(fp, "%d %s %d\n", students[i].num, students[i].name, students[i].score);
	}
	fclose(fp);
	printf("DB�� �����߽��ϴ�.\n");
}

void insert(char* num, char* name, char* score, Student* students) {
	students = (Student*)realloc(students, _msize(students) + sizeof(Student));
	int index = (_msize(students) / sizeof(Student)) - 1;
	students[index].num = atoi(num);
	strcpy(students[index].name, name);
	students[index].score = atoi(score);
	printf("�л� ������ �����߽��ϴ�.\n");
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
			printf("�л� ������ �����߽��ϴ�.\n");
			return;
		}
	}
}

void list(char* name, Student* students) {
	for (int i = 0; i < _msize(students) / sizeof(Student); i++) {
		if (!strcmp(students[i].name, name)) {
			printf("��ȣ : %d\n�̸� : %s\n���� : %d\n", students[i].num, students[i].name, students[i].score);
			return;
		}
	}
	printf("ã���� �ϴ� �л��� �����ϴ�.\n");
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
	printf("DB�� �̸������� �����߽��ϴ�.\n");
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
	printf("DB�� ���������� �����߽��ϴ�.\n");
}
