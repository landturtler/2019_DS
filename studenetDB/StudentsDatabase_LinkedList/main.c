#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <direct.h>
#include <io.h>

// �л� ����ü ����
typedef struct Node{
	// ����ü ������ : 20bytes(align 2bytes)
	int num;
	char name[10];
	int score;
	struct Node* left;
	struct Node* right;
} Student;

// �Լ� ���漱��
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
		printf("���ɾ �Է��ϼ���:");
		char commandLine[100];
		fgets(commandLine, 100, stdin);
		commandLine[strlen(commandLine) - 1] = '\0';

		if (!strcmp(commandLine, "exit")) {
			printf("���α׷��� �����մϴ�.\n");
			return 0;
		}

		// ���ɾ� �Ľ�
		char* firstCommand = strtok(commandLine, " ");

		// �Ľ� �� ���ɾ ���� ������ ����
		char* dbName;
		char* num;
		char* name;
		char* score;
		char* etc;

		// � ���ɾ����� ����
		if (!strcmp(firstCommand, "read")) {
			// read ����
			dbName = strtok(NULL, " ");
			students = readDB(dbName);
		}
		else if (!strcmp(firstCommand, "write")) {
			// write 
			dbName = strtok(NULL, " ");
			writeDB(dbName, students);
		}
		else if (!strcmp(firstCommand, "insert")) {
			// insert ����
			num = strtok(NULL, " ");
			name = strtok(NULL, " ");
			score = strtok(NULL, " ");
			clock_t start, end;
			start = clock();
			insert(num, name, score, students);
			end = clock();
			printf(" %lf�� �ɷȽ��ϴ�.\n",  (double)(end - start) / CLOCKS_PER_SEC);
		}
		else if (!strcmp(firstCommand, "delete")) {
			// delete ����
			name = strtok(NULL, " ");
			clock_t start, end;
			start = clock();
			students = delete(name, students);
			end = clock();
			printf("%lf�� �ɷȽ��ϴ�.\n", (double)(end - start) / CLOCKS_PER_SEC);
		}
		else if (!strcmp(firstCommand, "list")) {
			// list ����
			name = strtok(NULL, " ");
			list(name, students);
		}
		else if (!strcmp(firstCommand, "sort")) {
			// sort ����
			etc = strtok(NULL, " ");
			clock_t start, end;
			start = clock();
			if (!strcmp(etc, "name")) sortByName(students);
			else if (!strcmp(etc, "score")) sortByScore(students);
			else printf("�߸��� ���� ����Դϴ�.\n");
			end = clock();
			printf(" %lf�� �ɷȽ��ϴ�.\n", (double)(end - start) / CLOCKS_PER_SEC);
		}
		else {
			//���ܹ߻�
			printf("�������� ���� ���ɾ� �Դϴ�.\n");
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
	//���� ���� ���� �˻�
	// access�� ������ �̹� �����ϴ��� �˻��� �� �ִ� �Լ�.
	if (!access(dbName, 00)) {
		//������ �����ϴ� ��� ������ �д´�.
		printf("DB�� ã�ҽ��ϴ�.\n");
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
			// ���� �߰��� �л� �޸� �Ҵ�
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
		printf("DB�� �о����ϴ�.\n");
		return students;
	}
	else {
		printf("DB�� �������� �ʽ��ϴ�.\n");
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
	printf("DB�� �����߽��ϴ�.\n");
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
	printf("�л� ������ �����߽��ϴ�.\n");
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
			printf("�л� ������ �����߽��ϴ�.\n");
			return students;
		}
		ptr = ptr->right;
	}
	printf("ã���� �ϴ� �л��� �����ϴ�.\n");
	return students;
}

void list(char* name, Student* students) {
	Student* ptr = students;
	while(ptr != NULL) {
		if (!strcmp(ptr->name, name)) {
			printf("��ȣ : %d\n�̸� : %s\n���� : %d\n", ptr->num, ptr->name, ptr->score);
			return;
		}
		ptr = ptr->right;
	}
	printf("ã���� �ϴ� �л��� �����ϴ�.\n");
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
	printf("DB�� �̸������� �����߽��ϴ�.\n");
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
	printf("DB�� ���������� �����߽��ϴ�.\n");
}