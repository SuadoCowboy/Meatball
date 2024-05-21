import os
import shutil

def copyToExampleBuild(fromPath: str):
    if os.path.exists(fromPath) and not os.path.isdir(fromPath):
        shutil.copy(fromPath, os.path.join('build', 'examples'))
        return
    
    shutil.copytree(fromPath, os.path.join('build', 'examples', os.path.basename(fromPath)), dirs_exist_ok=True)

if __name__ == '__main__':
    copyToExampleBuild(os.path.join('build', 'libMeatball.dll'))
    copyToExampleBuild(os.path.join('build', 'libMeatball.dll.a'))
    copyToExampleBuild(os.path.join('examples', 'data'))