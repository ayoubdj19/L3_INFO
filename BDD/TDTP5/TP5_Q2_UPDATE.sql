UPDATE LesPersonnes SET salaire=(salaire*1.10)
WHERE noP IN(SELECT noP
            FROM LesChauffeurs
            WHERE nbK > 400);
