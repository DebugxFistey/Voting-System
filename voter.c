#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct ElectionSchedule {
    char constituency[50];
    char date[11];
};

struct Candidate {
    char name[50];
    char party[50];
    char constituency[50];
};

struct Voter {
    int sno;
    char name[50];
    char dob[11];
    char address[100];
    char password[20];
};

struct Vote {
    int voterSno;
    char candidateName[50];
};

const char ADMIN_USER[] = "user";
const char ADMIN_PASSWORD[] = "adminpass";

int generateVoterSNO() {
    srand(time(0));
    int randomNum = rand() % 900000 + 100000;
    return randomNum;
}

void addElectionSchedule() {
    FILE *electionScheduleFile = fopen("schedule.txt", "a+");
    char adminPassword[20];
    printf("Enter Admin Password: ");
    scanf("%s", adminPassword);

    if (strcmp(adminPassword, ADMIN_PASSWORD) != 0) {
        printf("Incorrect admin password. Access denied.\n");
        return;
    }

    struct ElectionSchedule schedule;
    printf("Enter Constituency: ");
    scanf("%s", schedule.constituency);
    printf("Enter Date of Election (yyyymmdd): ");
    scanf("%s", schedule.date);

    // Save election schedule to the file
    fprintf(electionScheduleFile, "%s %s\n", schedule.constituency, schedule.date);
    printf("Election schedule added successfully!\n");
    fclose(electionScheduleFile);
}

void addCandidate() {
    FILE *candidateFile = fopen("candidatelist.txt", "a+");
    char adminPassword[20];
    printf("Enter Admin Password: ");
    scanf("%s", adminPassword);

    if (strcmp(adminPassword, ADMIN_PASSWORD) != 0) {
        printf("Incorrect admin password. Access denied.\n");
        return;
    }

    struct Candidate candidate;
    printf("Enter Name of Candidate: ");
    scanf("%s", candidate.name);
    printf("Enter Political Party: ");
    scanf("%s", candidate.party);
    printf("Enter Candidacy From: ");
    scanf("%s", candidate.constituency);

    fprintf(candidateFile, "%s %s %s\n", candidate.name, candidate.party, candidate.constituency);
    printf("Candidate added successfully!\n");
    fclose(candidateFile);
}

void registerVoter() {
    FILE *vFile = fopen("voterlist.txt", "a+");
    struct Voter voter;
    voter.sno = generateVoterSNO();
    printf("Enter Name of Voter: ");
    scanf("%s", voter.name);
    printf("Enter Date of Birth (yyyy/mm/dd): ");
    scanf("%s", voter.dob);
    printf("Enter Address: ");
    scanf("%s", voter.address);
    printf("Enter Password: ");
    scanf("%s", voter.password);

    fprintf(vFile, "%d %s %s %s %s", voter.sno, voter.name, voter.dob, voter.address, voter.password);
    printf("Voter registered successfully!\n");
    fclose(vFile);

}

void updateVoterDetails() {
    FILE *vFile = fopen("voterlist.txt", "a+");
    fseek(vFile, 0, SEEK_SET);

    FILE *tempFile = fopen("temp_voterlist.txt", "w");

    if (tempFile == NULL) {
        perror("Error opening temporary file");
        return;
    }

    int updated = 0;
    struct Voter voter;
    int voterId;
    printf("Enter the Voter Id:");
    scanf("%d", &voterId);

    while (fscanf(vFile, "%d %s %s %s %s", &voter.sno, voter.name, voter.dob, voter.address, voter.password) != EOF) {
        if (voter.sno == voterId) {
            printf("Enter new Date of Birth (yyyy/mm/dd): ");
            scanf("%s", voter.dob);
            printf("Enter new Address: ");
            scanf("%s", voter.address);
            updated = 1;
        }else
        {
            printf("Voter ID doesnot found");
        }

        fprintf(tempFile, "%d %s %s %s %s\n", voter.sno, voter.name, voter.dob, voter.address, voter.password);
    }

    fclose(vFile);
    fclose(tempFile);

    if (updated) {
        remove("voterlist.txt");
        rename("temp_voterlist.txt", "voterlist.txt");
        printf("Voter details updated successfully!\n");
    } else {
        remove("temp_voterlist.txt");
        printf("Voter details not found.\n");
    }
}

void changeVoterPasswordByAdmin() {
    FILE *vFile = fopen("voterlist.txt", "a+");
    fseek(vFile, 0, SEEK_SET);

    FILE *tempFile = fopen("temp_voterlist.txt", "w");

    if (tempFile == NULL) {
        perror("Error opening temporary file");
        return;
    }

    int updated = 0;
    struct Voter voter;
    int voterId;
    printf("Enter the Voter Id:");
    scanf("%d", &voterId);
    int found = 0;

    while (fscanf(vFile, "%d %s %s %s %s", &voter.sno, voter.name, voter.dob, voter.address, voter.password) != EOF) {
        if (voter.sno == voterId) {
            printf("Enter new Password: ");
            scanf("%s", voter.password);
            updated = 1;
            found = 1;
        }

        fprintf(tempFile, "%d %s %s %s %s\n", voter.sno, voter.name, voter.dob, voter.address, voter.password);
    }
    if (!found) {
        printf("Voter with SNO %d not found.\n", voterId);
    }

    fclose(vFile);
    fclose(tempFile);

    if (updated) {
        remove("voterlist.txt");
        rename("temp_voterlist.txt", "voterlist.txt");
        printf("Password updated successfully!\n");
    } else {
        remove("temp_voterlist.txt");
        printf("No matching record found.\n");
    }
}

void changePasswordByVoter(int loggedInVoterSno) {
    FILE *vFile = fopen("voterlist.txt", "a+");
    fseek(vFile, 0, SEEK_SET);

    FILE *tempFile = fopen("temp_voterlist.txt", "w");

    if (tempFile == NULL) {
        perror("Error opening temporary file");
        return;
    }

    int updated = 0;
    struct Voter voter;

    while (fscanf(vFile, "%d %s %s %s %s", &voter.sno, voter.name, voter.dob, voter.address, voter.password) != EOF) {
        if (voter.sno == loggedInVoterSno) {
            printf("Enter new Password: ");
            scanf("%s", voter.password);
            updated = 1;
        }

        fprintf(tempFile, "%d %s %s %s %s\n", voter.sno, voter.name, voter.dob, voter.address, voter.password);
    }

    fclose(vFile);
    fclose(tempFile);

    if (updated) {
        remove("voterlist.txt");
        rename("temp_voterlist.txt", "voterlist.txt");
        printf("Password updated successfully!\n");
    } else {
        remove("temp_voterlist.txt");
        printf("No matching record found.\n");
    }
}

void searchVoterDetails() {
    FILE *vFile = fopen("voterlist.txt", "a+");
    int searchSno;
    printf("Enter Voter SNO to search: ");
    scanf("%d", &searchSno);

    struct Voter voter;
    int found = 0;

    while (fscanf(vFile, "%d %s %s %s %s", &voter.sno, voter.name, voter.dob, voter.address, voter.password) != EOF) {
        if (voter.sno == searchSno) {
            printf("Voter Details:\n");
            printf("Voter SNO: %d\n", voter.sno);
            printf("Name: %s\n", voter.name);
            printf("Date of Birth: %s\n", voter.dob);
            printf("Address: %s\n", voter.address);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Voter with SNO %d not found.\n", searchSno);
    }
    fclose(vFile);
}

void voterDetails(int loggedInVoterSno) {
    FILE *vFile = fopen("voterlist.txt", "a+");

    struct Voter voter;
    int found = 0;

    while (fscanf(vFile, "%d %s %s %s %s", &voter.sno, voter.name, voter.dob, voter.address, voter.password) != EOF) {
        if (voter.sno == loggedInVoterSno) {
            printf("Voter Details:\n");
            printf("Voter SNO: %d\n", voter.sno);
            printf("Name: %s\n", voter.name);
            printf("Date of Birth: %s\n", voter.dob);
            printf("Address: %s\n", voter.address);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Voter with SNO %d not found.\n", loggedInVoterSno);
    }
    fclose(vFile);
}

bool checkIfVoterVoted(int loggedInVoterSno, FILE *voteFile) {
    struct Vote vote;
    fseek(voteFile, 0, SEEK_SET);

    while (fscanf(voteFile, "%d %s", &vote.voterSno, vote.candidateName) != EOF) {
        if (vote.voterSno == loggedInVoterSno) {
            return true;
        }
    }

    return false; // Voter has not cast a vote
}

bool checkIfValidCandidate(const char *candidateName, FILE *candidateFile) {
    struct Candidate candidate;

    fseek(candidateFile, 0, SEEK_SET);

    while (fscanf(candidateFile, "%s %s %s", candidate.name, candidate.party, candidate.constituency) != EOF) {
        if (strcmp(candidate.name, candidateName) == 0) {
            return true;
        }
    }

    return false;
}


void displayVoteResults() {
    FILE *vtFile = fopen("votecount.txt", "a+");
    struct CandidateVote {
        char candidateName[50];
        int voteCount;
    };

    struct CandidateVote candidateVotes[50];
    int candidateCount = 0;
    struct Vote vote;

    fseek(vtFile, 0, SEEK_SET);

    while (fscanf(vtFile, "%d %s", &vote.voterSno, vote.candidateName) != EOF) {
        int found = 0;
        for (int i = 0; i < candidateCount; i++) {
            if (strcmp(candidateVotes[i].candidateName, vote.candidateName) == 0) {
                candidateVotes[i].voteCount++;
                found = 1;
                break;
            }
        }

        if (!found) {
            strcpy(candidateVotes[candidateCount].candidateName, vote.candidateName);
            candidateVotes[candidateCount].voteCount = 1;
            candidateCount++;
        }
    }

    printf("Vote Results:\n");
    for (int i = 0; i < candidateCount; i++) {
        printf("%s: %d votes\n", candidateVotes[i].candidateName, candidateVotes[i].voteCount);
    }
    fclose(vtFile);
}


void castVote(int loggedInVoterSno) {
    FILE *vFile = fopen("voterlist.txt", "a+");
    FILE *cFile = fopen("candidatelist.txt", "a+");
    FILE *vtFile = fopen("votecount.txt", "a+");

    int hasVoted = checkIfVoterVoted(loggedInVoterSno, vtFile);
    if (hasVoted) {
        printf("You have already cast your vote.\n");
        return;
    }

    printf("List of Candidates:\n");
    displayCandidateList();

    char selectedCandidate[50];
    printf("Enter the name of the candidate you want to vote for: ");
    scanf("%s", selectedCandidate);

    int isValidCandidate = checkIfValidCandidate(selectedCandidate, cFile);
    if (!isValidCandidate) {
        printf("Invalid candidate selected.\n");
        return;
    }

    struct Vote vote;
    vote.voterSno = loggedInVoterSno;
    strcpy(vote.candidateName, selectedCandidate);
    fprintf(vtFile, "%d %s\n", vote.voterSno, vote.candidateName);

    printf("Vote cast successfully!\n");
    fclose(vFile);
    fclose(cFile);
    fclose(vtFile);
}

void displayCandidateList() {
    FILE *cFile = fopen("candidatelist.txt", "a+");
    struct Candidate candidate;

    fseek(cFile, 0, SEEK_SET);

    printf("List of Candidates:\n");

    while (fscanf(cFile, "%s %s %s", candidate.name, candidate.party, candidate.constituency) != EOF) {
        printf("Name: %s\t", candidate.name);
        printf("Party: %s\t", candidate.party);
        printf("Candidacy From: %s\t", candidate.constituency);
        printf("\n");
    }
    fclose(cFile);
}

void displayElectionSchedule() {
    FILE *eScheduleFile = fopen("schedule.txt", "a+");

    struct ElectionSchedule schedule;

    fseek(eScheduleFile, 0, SEEK_SET);

    printf("Election Schedule List:\n");

    while (fscanf(eScheduleFile, "%s %s", schedule.constituency, schedule.date) != EOF) {
        printf("Constituency: %s\n", schedule.constituency);
        printf("Date of Election: %s\n", schedule.date);
        printf("\n");
    }
}


int main() {
    FILE *voterFile = fopen("voterlist.txt", "a+");
    FILE *candidateFile = fopen("candidatelist.txt", "a+");
    FILE *electionScheduleFile = fopen("schedule.txt", "a+");
    FILE *voteFile = fopen("votecount.txt", "a+");

    if (voterFile == NULL || candidateFile == NULL || electionScheduleFile == NULL ||
        voteFile == NULL ) {
        perror("Error opening file");
        return 1;
    }

    int choice;
    int loggedIn = 0;
    int isAdmin = 0;
    int loggedInVoterSno = -1;

    printf("Welcome to the Voting System!\n");

    while (1) {
        if (!loggedIn) {
            char username[20];
            char password[20];
            printf("Enter Username: ");
            scanf("%s", username);
            printf("Enter Password: ");
            scanf("%s", password);

            if (strcmp(username, "admin") == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
                loggedIn = 1;
                isAdmin = 1;
                printf("Admin logged in.\n");
            } else {
                rewind(voterFile);
                struct Voter voter;
                while (fscanf(voterFile, "%d %s %s %s %s", &voter.sno, voter.name, voter.dob, voter.address, voter.password) != EOF) {
                    if (strcmp(username, voter.name) == 0 && strcmp(password, voter.password) == 0) {
                        loggedIn = 1;
                        isAdmin = 0;
                        loggedInVoterSno = voter.sno;
                        printf("Voter %s logged in.\n", voter.name);
                        break;
                    }
                }
            }
        } else {
            printf("Main Menu:\n");

            if (isAdmin) {
                printf("1. Register Voter\n");
                printf("2. Add Candidate\n");
                printf("3. Add Election Schedule\n");
                printf("4. Update Voter Details\n");
                printf("5. Change Voter Password\n");
                printf("6. Search Voter Details\n");
                printf("7. Display Vote Results\n");
                printf("8. Display Candidate List\n");
                printf("9. Display Election Schedule List\n");
                printf("10. Logout / Exit\n");
            } else {
                printf("1. Display Own Information\n");
                printf("2. Display Candidate List\n");
                printf("3. Display Election Schedule List\n");
                printf("4. Cast Vote\n");
                printf("5. Display Vote Results\n");
                printf("6. Change Password\n");
                printf("7. Logout / Exit\n");

            }

            printf("Enter your choice: ");
            scanf("%d", &choice);

            if (isAdmin) {
                switch (choice) {
                    case 1:
                        registerVoter();
                        break;
                    case 2:
                        addCandidate();
                        break;
                    case 3:
                        addElectionSchedule();
                        break;
                    case 4:
                        updateVoterDetails();
                        break;
                    case 5:
                        changeVoterPasswordByAdmin();
                        break;
                    case 6:
                        searchVoterDetails();
                        break;
                    case 7:
                        displayVoteResults();
                        break;
                    case 8:
                        displayCandidateList();
                        break;
                    case 9:
                        displayElectionSchedule();
                        break;
                    case 10:
                        printf("Logout...\n");
                        fclose(voteFile);
                        exit(0);
                        break;
                    default:
                        printf("Invalid choice!\n");
                }
            } 
            else if(!isAdmin){
                switch (choice) {
                    case 1:
                        voterDetails(loggedInVoterSno);
                        break;
                    case 2:
                        displayCandidateList();
                        break;
                    case 3:
                        displayElectionSchedule();
                        break;
                    case 4:
                        castVote(loggedInVoterSno);
                        break;
                    case 5:
                        displayVoteResults();
                        break;
                    case 6:
                        changePasswordByVoter(loggedInVoterSno);
                        break;
                    case 7:
                        printf("Logout...\n");
                        fclose(voteFile);
                        exit(0);
                        break;
                    default:
                        printf("Invalid choice!\n");
                }
            }
            
            else {
                printf("Access denied for this option!\n");
            }
        }
    }

    return 0;
}
