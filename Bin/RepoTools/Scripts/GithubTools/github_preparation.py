#!/usr/bin/env python
import sys
import os
import argparse

import subprocess

class Preparation:

    white_list_path = []
    delete_files = []

    def __init__(self, base_path, repo):
        self.repo = repo
        self.base_path = os.path.abspath(base_path)
        self.get_white_list()
        self.get_delete_list()
        self.remove_extra()

    def scan_paths(self):
        paths = []

        for root, dirs, files in os.walk(self.base_path):
            for file in files:
                paths.append(os.path.join(root, file))

        return paths

    def get_white_list(self):
        try:
            files_list = open('white_list.txt')
            for line in files_list:
                path = line.partition('#')[0]
                if len(path) != 0:
                    path = os.path.abspath('../../../../' + path.replace('\n', ''))
                    if path != self.base_path:
                        self.white_list_path.append(path)
        except IOError:
            print "##teamcity[message text='Unable to open file white_list.txt' status='ERROR']"
            sys.exit(3)

    def get_delete_list(self):
        root_path = self.scan_paths()
        sys.stdout.write('Deleting files from repo:\n')
        sys.stdout.flush()

        for path in root_path:
            if not [True for white_path in self.white_list_path if white_path in path]:
                sys.stdout.write(path.replace(self.base_path, '') + '\n')
                sys.stdout.flush()
                self.delete_files.append(path)

        self.delete_files.append(os.path.abspath(self.base_path + '/Bin/RepoTools/Scripts/GithubTools/white_list.txt'))

    def remove_extra(self):

        for path in self.delete_files:

            os.chdir(os.path.dirname(path))

            cmd = 'java -jar {}/bfg-1.12.15.jar --no-blob-protection --delete-files {} {}'.format(self.base_path + '/Bin/RepoTools/Scripts/GithubTools', os.path.basename(path), self.repo)

            sys.stdout.write(cmd + '\n')
            sys.stdout.flush()
            try:
                cmd_log = subprocess.check_output(cmd, shell=True)
                sys.stdout.write(cmd_log)
                sys.stdout.flush()
                if 'Usage: bfg' in cmd_log:
                    print "##teamcity[message text='Error removing big files' errorDetails='Not passed important parameters' status='ERROR']"
                    sys.exit(3)
            except subprocess.CalledProcessError as cmd_except:
                print "##teamcity[message text='Error removing extra files' errorDetails='%s' status='ERROR']" % cmd_except.output
                sys.exit(3)

    def remove_big_files(self):
        cmd = 'java -jar {}/bfg-1.12.15.jar --no-blob-protection --strip-blobs-bigger-than 99M {}'.format(self.base_path + '/Bin/RepoTools/Scripts/GithubTools', self.repo)
        sys.stdout.write(cmd + '\n')
        sys.stdout.flush()
        try:
            cmd_log = subprocess.check_output(cmd, shell=True)
            sys.stdout.write(cmd_log)
            sys.stdout.flush()
            if 'Usage: bfg' in cmd_log:
                print "##teamcity[message text='Error removing big files' errorDetails='Not passed important parameters' status='ERROR']"
                sys.exit(3)
        except subprocess.CalledProcessError as cmd_except:
            print "##teamcity[message text='Error removing big files' errorDetails='%s' status='ERROR']" % cmd_except.output
            sys.exit(3)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Utility tool to delete files from the repository that are not in the whitelist.')
    parser.add_argument('--path', required=True, help='Example: --path /var/git/repo')
    parser.add_argument('--repo', required=True, help='Example: --repo my-repo.git')

    args = parser.parse_args()

    preparation = Preparation(args.path, args.repo)


