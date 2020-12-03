CREATE TABLE `dbtify`.`song` (
  `id` INT NOT NULL,
  `title` VARCHAR(200) NOT NULL,
  `album` INT NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `album_idx` (`album` ASC) VISIBLE,
    FOREIGN KEY (`album`)
    REFERENCES `dbtify`.`album` (`id`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION);
