DELETE FROM LesPersonnes
WHERE noP NOT IN(
    SELECT noP
    FROM LesContrats
);