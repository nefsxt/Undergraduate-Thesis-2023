% ==================================
% DATASET GENERATION ENGINE
% ==================================

#the code generally produces 10 datasets for each case (1-4) 
#so in total when running it for all 4 cases it produces 40 trader ranking files and 40 customer weight files


clc;
clear variables;

% -----------------------------

% Number of datasets to generate per case
numDS = 10;
fprintf('-------------------------------------------\n');

% Dataset case selection (choices: 1-4)
# 4:4 runs only case 4 -> change to 1 for all cases or 1:1 for case 1, 2:2 for only 2 and so on
for caseID = 4:4
    
    fprintf('CASE %d \n',caseID);
    fprintf('-------------------------------------------\n');
    
    for nd = 1:numDS
        
        fprintf('***** Generating Dataset #%d ***** \n\n',nd);

        switch caseID
            case 1
                % Boutique trading firm
                N = 10;    
                Ns = 2;
                M = 50;
                Ms = 10;
            case 2
                % Discount brokerage firm
                N = 100;    
                Ns = 20;
                M = 1500;
                Ms = 250;
            case 3
                % Full-service brokerage firm
                N = 500;     
                Ns = 100;
                M = 7000;  
                Ms = 700;
            case 4
                % Online trading firm (e*trade)
                N = 1000;
                Ns = 250;
                M = 50000; 
                Ms = 5000;
            otherwise
                error('..... Unknown case ID [caseID=%d] \n\n',caseID);
        end
        
        % Get number of traders and customers
        NT = round(N + Ns*randn);
        MT = round(M + Ms*randn);
        
        % Call dataset builder
        build_dataset(caseID,NT,MT);
        
    end
end
fprintf('All datasets generation finished \n');