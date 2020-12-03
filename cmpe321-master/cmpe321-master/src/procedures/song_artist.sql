CREATE TABLE `dbtify`.`song_artist` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `song_id` INT NOT NULL,
  `artist_id` INT NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `song_idx` (`song_id` ASC) VISIBLE,
  INDEX `artist_idx` (`artist_id` ASC) VISIBLE,

  FOREIGN KEY (`song_id`)
    REFERENCES `dbtify`.`song` (`id`)
    ON DELETE CASCADE,

  FOREIGN KEY (`artist_id`)
    REFERENCES `dbtify`.`artist` (`id`)
    ON DELETE CASCADE);

